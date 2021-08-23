/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Valores de ADC mediante LCD y EUSART
 * Hardware: Pic 16f887, potenciometro, comunicación serial
 * 
 * Created on 25 de julio 2021, 10:30
 * Last modification 25 de julio 2021, 1 
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
#include <stdio.h>
#include "valor_ADC.h"
#include "LCD_16_2.h"
#include "USART.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------

char dato1, dato2;
double voltaje1, voltaje2, voltaje3;
char valor1[6];
char valor2[6];
char valor3[6];
char comp1, comp2;
char ingreso;
float noentero;
char prr;
//--------------------------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    LCD_Init8();
    USART_Init();
    ANSEL = 0b01100000; //pin 5 y 6 analogicos
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0x00;
    TRISC = 0b10000000;
    TRISD = 0x00;
    TRISE = 0b111;     //pin 0 y 1 entradas
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                           
                            //Config. ADC
    ADCON1bits.ADFM = 0;    //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;   //voltaje de 0V-5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS0 = 0;   //Fosc/32
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.CHS = 5;     //canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;    //activo el modulo
    
    ADCON0bits.GO = 1;  
    
                           //Estado inicial
    dato1 = 0;
    dato2 = 0;
    voltaje1 = 0;
    voltaje2 = 0;
    
                           //Estado inicial
    PORTA = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    noentero = 4.2;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        comp1 = dato1;
        comp2 = dato2;
        
        if(ADCON0bits.GO == 0){
            
            if(ADCON0bits.CHS == 6){
                dato2 = ADRESH;
                ADCON0bits.CHS = 5;
            }
            else if(ADCON0bits.CHS == 5){
                dato1 = ADRESH;
                ADCON0bits.CHS = 6;
            }
            __delay_us(50);     //con 6 micros segundos será suficiente se dejo
                                //en 50 por fallos de software en proteus
            ADCON0bits.GO = 1;
        }
        voltaje1 = (dato1 * 0.0196);    //porque solo uso el adresh
        voltaje2 = (dato2 * 0.0196);    //porque solo uso el adresh
        voltaje3 = (PORTA * 0.0196);    //porque solo uso el adresh
        
        sprintf(valor1, "%3.2fv", voltaje1);
        sprintf(valor2, "%3.2fv", voltaje2);
        sprintf(valor3, "%3.2fv", voltaje3);
        
        
        LCD_8XY(0,0);                //donde aparecera
        LCD_8Cadena("S1:   S2:   S3:");
               
        LCD_8XY(1,0);
        LCD_8Cadena(valor1);
        LCD_8XY(1,6);
        LCD_8Cadena(valor2);
        LCD_8XY(1,12);
        LCD_8Cadena(valor3);
        
        if(comp1 != dato1 || comp2 != dato2){
            USART_Cadena("\n\r\n\r+ para aumentar contador\n\r");
            USART_Cadena("- para disminuir contador\n\r\n\r");
            USART_Cadena("Voltaje pot1: ");
            USART_Cadena(valor1);
       
            USART_Cadena("\n\rVoltaje pot2: ");
            USART_Cadena(valor2);
            
            
            prr = (uint16_t)(noentero);
          
        }
        
        if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
            ingreso = USART_Rx();
       }
       
        if(ingreso == '+'){
            PORTA++;
        }else if(ingreso == '-'){
            PORTA--;
        }
        ingreso = 0;
        
    }
    return;
}

