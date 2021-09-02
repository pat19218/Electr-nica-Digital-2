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
 * Last modification 23 de agosto 2021, 17:21
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
uint8_t DataBuffer[6];

uint32_t Raw_temperatura;
int temperatura;
uint32_t Raw_humedad;
int humedad_tem;
char humedad;

char entero, decimal;
char cen, dec, uni;

char ingreso, actuador;

//--------------------------funciones-------------------------------------------
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);
void Init_AHT10 (void);
//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    USART_Init();
    ANSEL = 0b01100000; //pin 5 y 6 analogicos
    ANSELH = 0x00;      // solo pines digitales
    
    TRISC = 0b10000000;  
    TRISE = 0b111;     //pin 0 y 1 entradas
    
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
    I2C_Master_Init(100000); // Inicializar Comuncación I2C
    Init_AHT10();
    
    
    
    PORTA = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    LCD_Begin(0x40);    // Initialize LCD module with I2C address = 0x4
    LCD_Goto(1, 1);     // Go to column 2 row 1
    LCD_Print("Hello, world!");
    
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        
        //inicio de medición del sensor
        I2C_Master_Start();
        I2C_Master_Write(0x70); //inicio comunicación   addres can be 0x70
        I2C_Master_Write(0xAC); //comando de incio de medicion
        I2C_Master_Write(0x33); //primer dato (humedad)
        I2C_Master_Write(0x00); //segundo dato (temperatura)
        I2C_Master_Stop();
        __delay_ms(75);         //de almenos 75mS
        
        //lectura en loop principal
        I2C_Master_Start();
        I2C_Master_Write(0x71); //ubicacion para lectura
        DataBuffer[0] = I2C_Master_Read(0); //humedad
        DataBuffer[1] = I2C_Master_Read(0); //humedad
        DataBuffer[2] = I2C_Master_Read(0); //humedad y temperatura
        DataBuffer[3] = I2C_Master_Read(0); //temperatura
        DataBuffer[4] = I2C_Master_Read(0); //temperatura
        DataBuffer[5] = I2C_Master_Read(0); //temperatura
        I2C_Master_Stop();
        __delay_ms(200);
        
        //soft reset
//        I2C_Master_Start();
//        I2C_Master_Write(0x70); //inicio comunicación   addres can be 0x70
//        I2C_Master_Write(0xBA); //comando de reinicio suave
//        I2C_Master_Stop();
//        __delay_ms(20);
        
        Raw_temperatura = ((uint32_t)(DataBuffer[3] & 0x0F) <<16); 
        Raw_temperatura |=((uint16_t)DataBuffer[4]<<8); 
        Raw_temperatura |=DataBuffer[5]; //20 bits de datos
        temperatura = Raw_temperatura * 200 / 1048576 - 50; //temperatura en celcius
        
         //inicio de medición del sensor
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        humedad_tem = ((I2C_Master_Read(0))<<8);
        humedad_tem += I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        humedad = 175 - ((25/22)*humedad_tem);  //humedad

        //aseguro rango de humedad y esta con una presicion del 2%
        if(humedad < 0){humedad = 0;}
        if(humedad > 100){humedad = 100;}
        
       
        LCD_Goto(1, 2);             // Go to column 2 row 1
        LCD_Print(&humedad);
        LCD_Print("%");
        
        LCD_Goto(7, 2);             // Go to column 2 row 1
        LCD_Print(&temperatura);
        LCD_Goto(11, 2);             // Go to column 2 row 1
        LCD_Print("°C");
        
        if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
            ingreso = USART_Rx();
       }
        if(ingreso == 'h'){
            USART_Cadena(&humedad);//humedad
        }
        if(ingreso == 't'){
            USART_Cadena(&temperatura);//temperatura
        }
        ingreso = 0;
        
        switch(temperatura){     //temperatura
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
        
        if (humedad < 22){       //humedad
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

void Init_AHT10 (void){
    //0x38 ubicacion del sensor  0x39 lectura del sensor
    PORTCbits.RC3 = 1;   
    __delay_ms(20);
    
    //Init medidas
    I2C_Master_Start();
    I2C_Master_Write(0x70); //inicio comunicación   addres can be 0x70
    I2C_Master_Write(0x70); //inicio comunicación   addres can be 0x70
    I2C_Master_Write(0xE1); //comando de inicio
    I2C_Master_Write(0xAC); //comando medicion de temperatura y humedad
    I2C_Master_Write(0x08); 
    I2C_Master_Write(0x00); 
    I2C_Master_Stop();
    __delay_ms(350);
    
}
