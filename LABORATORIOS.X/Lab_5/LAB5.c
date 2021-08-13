/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Pic y IO
 * Hardware: Pic 16f887, leds, comunicación serial
 * 
 * Created on 13 de agosto 2021, 14:43
 * Last modification 
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
#include "USART.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------

//-----------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    if(RBIF == 1){
        if (RB0 == 0){
            PORTD++;
        }else if(RB1 == 0){
            PORTD--;
        }
        RBIF = 0;
    }
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    USART_Init();
    ANSEL = 0x00;
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0x00;
    TRISB = 0x03;
    TRISC = 0x00;
    TRISD = 0x00;
    
    
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
    
    INTCONbits.GIE = 1;     //habilito interrupciones
    INTCONbits.RBIE = 1;    //activo interrupciones por cambio de estado
    INTCONbits.RBIF = 0;    //bajo la bandera
    
                           //Estado inicial
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        
    }
    return;
}

