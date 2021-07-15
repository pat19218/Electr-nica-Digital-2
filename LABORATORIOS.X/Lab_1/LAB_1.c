/*
 * File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Dispensador de pastillas con sistema de ventilacion y usuario
 * Hardware: Pic 16f887, transistores, resistencias, leds, button
 * 
 * Created on 13 de julio 2021, 18:31
 * Last modification 13 de julio 2021, 
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

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
                          // 0     1    2     3     4
const char num_display[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66,
                            0xB6, 0xBE, 0xE0, 0xFE, 0xF6,
                          //  5     6     7    8     9
                          //  A     B     C    D     E    F
                            0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E};
char ref;
char turno;
char valor_adc;
char dato_mayor, dato_menor;
//--------------------------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
    if(RBIF == 1){
        if (RB0 == 0){
            ref++;
        }else if(RB1 == 0){
            ref--;
        }
        RBIF = 0;
    }
    
    else if(T0IF == 1){
        //asm("NOP");
        PORTD = 0x00;
        
        if (turno == 1){
            PORTC = (num_display[dato_mayor]);
            RD1 = 1;
            turno = 0;
        }else if(turno == 0){
            PORTC = (num_display[dato_menor]);
            RD0 = 1;
            turno = 1;
        }
        
        T0IF = 0;
        TMR0 = 60;                  //Reinicio del timmer (7ms)
    }
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    ANSEL = 0b100000000;
    ANSELH = 0x00;     // solo pines digitales
    
    TRISA = 0x00;    
    TRISB = 0b00000011;    
    TRISC = 0x00;    
    TRISD = 0x00;    
    TRISE = 0b0100;    
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                            //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
                        //Config. interrupcion RB
    IOCBbits.IOCB0 = 1; // pines en lo que habra interrupcion por cambio 
    IOCBbits.IOCB1 = 1; //de estado
    RBIF = 0;
    
                                //Config. timmer0 
    OPTION_REGbits.T0CS = 0;    //Uso reloj interno
    OPTION_REGbits.PSA = 0;     //Uso pre-escaler
    OPTION_REGbits.PS = 0b111;  //PS = 111 / 1:256
    TMR0 = 60;                 //Reinicio del timmer (7ms)

                            //Config. ADC
    ADCON1bits.ADFM = 0;    //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;   //voltaje de 0V-5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS0 = 0;   //Fosc/32
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.CHS = 7;     //canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;    //activo el modulo
    
    
    INTCONbits.GIE = 1;     //habilito interrupciones
    INTCONbits.RBIE = 1;    //activo interrupciones por cambio de estado
    INTCONbits.RBIF = 0;    //bajo la bandera
    INTCONbits.T0IE = 1;    //activo interrupciones por timmer 0
    INTCONbits.T0IF = 0;    //bajo la bandera
    
    ADCON0bits.GO = 1;  
    
    ref = 0;                //Estado inicial
    dato_mayor = 0;
    dato_menor = 0;
    
    PORTA = 0x00;           //Estado inicial
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        PORTA = ref;
        
        if(ADCON0bits.GO == 0){     //convierto en digital el analog
          valor_adc = ADRESH;  
        __delay_us(50);         //con 6 micros segundos será suficiente se dejo
                                //en 50 por fallos de software en proteus
        ADCON0bits.GO = 1;
        }
        
        dato_mayor = hex_mayor(valor_adc);
        dato_menor = hex_menor(valor_adc);
                  
        RB6 = (valor_adc > ref) ? 1:0;
        
    }
    return;
}