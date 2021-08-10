/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Valores de ADC mediante LCD e I2C. ESCLAVO
 * Hardware: Pic 16f887, potenciometro, comunicación I2C
 * 
 * Created on 8 de agosto de 2021, 8:20
 * Last modification 8 de agosto de 2021, 
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
#include "I2C.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
char z;
char old1, old2;

//--------------------------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
    if(PIR1bits.SSPIF == 1){ //interrupcion protocolo I2C

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTA = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;         // Lectura del SSBUF para limpiar el buffer y la bandera BF
            BF = 0;
            SSPBUF = PORTD;     //envio el dato al maestro
            SSPCONbits.CKP = 1; // Habilita entrada de pulsos de reloj SCL
            __delay_us(250);
            while(SSPSTATbits.BF); // Esperar a que la recepción se complete
        }
       
        PIR1bits.SSPIF = 0;    
    }//fin interrupcion protocolo I2C
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
   
    ANSEL = 0x00;
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0x00;
    TRISD = 0x00;       //solo pines de salida a exception 
    TRISB = 0b00000011; //solo pines de salida a exception de los primeros 2
    
    OSCCONbits.IRCF = 0b111;    //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;         //reloj interno
    
                                //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;                       
    
    
                           //Estado inicial
    PORTD = 0x00;
    PORTB = 0x00;    
    PORTA = 0x00;    
    old1 = 1;
    old2 = 1;
    I2C_Slave_Init(0x60);   
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        if(RB0 == 0 && old1 != RB0){
            PORTD++;
        }
        if(RB1 == 0 && old2 != RB1){
            PORTD--;
        }
        old1 = RB0;
        old2 = RB1;
        PORTD &= 0x0F;
    }
    return;
}

