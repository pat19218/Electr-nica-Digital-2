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
#include "valor_ADC.h"
#include "LCD_16_2.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
                          // 0     1    2     3     4
const char num_display[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66,
                            0xB6, 0xBE, 0xE0, 0xFE, 0xF6,
                          //  5     6     7    8     9
                          //  A     B     C    D     E    F
                            0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E};
char valor_adc;
char dato_mayor_1, dato_menor_1, dato_mayor_2, dato_menor_2;
//--------------------------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    LCD_Init8();
    ANSEL = 0b01100000; //pin 5 y 6 analogicos
    ANSELH = 0x00;      // solo pines digitales
       
    TRISE = 0b0011;     //pin 0 y 1 entradas
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                           
                            //Config. ADC
    ADCON1bits.ADFM = 0;    //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;   //voltaje de 0V-5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS0 = 0;   //Fosc/32
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.CHS = 7;     //canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;    //activo el modulo
    
    ADCON0bits.GO = 1;  
    
                           //Estado inicial
    dato_mayor_1 = 0;
    dato_menor_1 = 0;
    dato_mayor_2 = 0;
    dato_menor_2 = 0;
    
                           //Estado inicial
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        
        if(ADCON0bits.GO == 0){
            
            if(ADCON0bits.CHS == 6){
                valor_adc = ADRESH;
                ADCON0bits.CHS = 5;
                dato_mayor_1 = hex_mayor(valor_adc);
                dato_menor_1 = hex_menor(valor_adc);
            }
            else if(ADCON0bits.CHS == 5){
                valor_adc = ADRESH;
                ADCON0bits.CHS = 6;
                dato_mayor_2 = hex_mayor(valor_adc);
                dato_menor_2 = hex_menor(valor_adc);
            }
            __delay_us(50);     //con 6 micros segundos será suficiente se dejo
                                //en 50 por fallos de software en proteus
            ADCON0bits.GO = 1;
        }
        
        LCD_XY(0,0);        //dond aparecera
        LCD_Cadena("veamos :)");
        __delay_ms(100);
        LCD_XY(1,9);    
        LCD_Cadena("wowowow");
        __delay_ms(100);
    }
    return;
}

