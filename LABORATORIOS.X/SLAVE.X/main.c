/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Valores de ADC mediante SPI "ESCLAVO"
 * Hardware: Pic 16f887, potenciometro, comunicación SPI
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
#include "SPI.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
char dato1, dato2;
char turno; 
//--------------------------funciones-------------------------------------------

//---------------------------interrupciones-------------------------------------
void __interrupt()isr(void){
    if(SSPIF == 1){
        turno = spiRead();
        if(turno == 0x01){
            spiWrite(dato1);
        }else{
            spiWrite(dato2);
        }
    }
    SSPIF = 0;
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    ANSEL = 0b00000011; //pin 0 y 1 analogicos
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0b00000011; //pin 0 y 1 entradas y A5 --> Slave Select
    TRISAbits.TRISA5 = 1;       // Slave Select
    
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                           
                            //Config. ADC
    ADCON1bits.ADFM = 0;    //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;   //voltaje de 0V-5V
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS0 = 0;   //Fosc/32
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.CHS = 0;     //canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;    //activo el modulo
    
    ADCON0bits.GO = 1;  
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
        
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
                            //Estado inicial
    dato1 = 0;
    dato2 = 0;
                           //Estado inicial
    PORTA = 0x00;
    PORTC = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        if(ADCON0bits.GO == 0){
            
            if(ADCON0bits.CHS == 1){    //leo dato ch 1 y cambio a ch0
                dato2 = ADRESH;
                ADCON0bits.CHS = 0;
            }
            else if(ADCON0bits.CHS == 0){//leo dato de ch 0 y cambio a ch 1
                dato1 = ADRESH;
                ADCON0bits.CHS = 1;
            }
            __delay_us(50);     //con 6 micros segundos será suficiente se dejo
                                //en 50 por fallos de software en proteus
            ADCON0bits.GO = 1;
        }
        
    }
    return;
}

