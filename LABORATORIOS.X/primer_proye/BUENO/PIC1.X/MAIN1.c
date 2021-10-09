/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Pic y IO maestro
 * Hardware: Pic 16f887, leds, comunicación serial
 * 
 * Created on 23 de agosto 2021, 14:43
 * Last modification 02 de septiembre 2021, 12:48
 */

// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//-------------------------Librerias--------------------------------------------
#include <xc.h>
#include <stdint.h>
#include "USART.h"
#include "I2C.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
int valor_adc1, valor_adc2;
char cadena1[4];
char cadena2[5];
char envio[4];
char coso, ingreso;
char actuador;

//--------------------------funciones-------------------------------------------
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);


//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    USART_Init();
    ANSEL = 0b11100000; //pin 5 y 6 analogicos
    ANSELH = 0x00;      // solo pines digitales
    
    TRISC = 0b10000000;  
    TRISE = 0b011;     //pin 0 y 1 entradas
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                            //Config. ADC
    ADCON1bits.ADFM = 0;    //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;   //voltaje de 0V-5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS0 = 0;   //Fosc/32
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.CHS = 5;     //canal 5 pin 8
    __delay_us(100);
    ADCON0bits.ADON = 1;    //activo el modulo
    
    ADCON0bits.GO = 1;
    
    
                                //Estado inicial
    I2C_Master_Init(100000);    // Inicializar Comuncación I2C
    LCD_Begin(0x4E);            // Initialize LCD module with I2C address = 0x4
    LCD_Goto(1, 1);             // Go to column 1 row 1
    LCD_Print("Hum:  Tem: CP&JP");
    __delay_ms(1000);
    
    PORTC = 0x00;
    PORTE = 0x00;
    coso = 0xDF;  //es el cosito de la temperatura --> ° o presiona alt+223 -->▀

    //------------------------------loop principal----------------------------------
    while (1){
        
        if(ADCON0bits.GO == 0){
            
            
            if(ADCON0bits.CHS == 6){
                valor_adc2 = ADRESH;    //temperatura
                                
                cadena2[0] = (centenas(valor_adc2) + 48);
                cadena2[1] = (decenas(valor_adc2) + 48);
                cadena2[2] = (unidades(valor_adc2) + 48);
                cadena2[3] = coso;
                cadena2[4] = '\0';
                
                envio[0] = (centenas(valor_adc2) + 48);
                envio[1] = (decenas(valor_adc2) + 48);
                envio[2] = (unidades(valor_adc2) + 48);
                envio[3] = '\0';
                
                ADCON0bits.CHS = 5;
            }
            else if(ADCON0bits.CHS == 5){
                valor_adc1 = ADRESH;
                valor_adc1 = 175 - ((25/22)*valor_adc1);  //humedad
        
                if(valor_adc1 < 0){valor_adc1 = 0;}
                if(valor_adc1 > 100){valor_adc1 = 100;}
          
                cadena1[0] = (centenas(valor_adc1) + 48);
                cadena1[1] = (decenas(valor_adc1) + 48);
                cadena1[2] = (unidades(valor_adc1) + 48);
                cadena1[3] = '\0';
        
                ADCON0bits.CHS = 6;
            }
            
            ADCON0bits.GO = 1;
            __delay_us(50);     //con 6 micros segundos será suficiente se dejo
                                //en 50 por fallos de software en proteus
        }

        LCD_Goto(1, 2);             // Go to column 2 row 1
        LCD_Print(cadena1);
        LCD_Print("%");
        
        LCD_Goto(7, 2);             // Go to column 2 row 1
        LCD_Print(cadena2);
        LCD_Goto(11, 2);             // Go to column 2 row 1
        LCD_Print("C");
        
        if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
            ingreso = USART_Rx();
       }
        if(ingreso == 'h'){
            USART_Cadena(cadena1);//humedad
        }
        if(ingreso == 't'){
            USART_Cadena(envio);//temperatura
        }
        ingreso = 0;
        
        switch(valor_adc2){     //temperatura
            case(28):
                actuador = 0b00000011;
                break;
            case(26):
                actuador = 0b00000010;
                break;
            case(24):
                actuador = 0b00000001;
                break;
            case(22):
                actuador = 0b00000000;
                break;
        }
        
        if (valor_adc1 < 22){       //humedad
            actuador = actuador | 0b00000100;
        }
        
        
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(actuador);
        I2C_Master_Stop();
        
         __delay_ms(500);                
        
    }
    return;
}



char centenas (int dato){
    char out = dato / 100;
    return out;
}

char decenas (int dato){
    char out;
    out = (dato % 100) / 10;
    return out;
}

char unidades (int dato){
    char out;
    out = (dato % 100) % 10;
    return out;
}