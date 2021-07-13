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
 * Created on 11 de julio 2021, 16:35
 * Last modification 12 de julio 2021, 19:42
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

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
                          // 0     1    2     3     4
const char num_display[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66,
                            0xB6, 0xBE, 0xE0, 0xFE, 0xF6};
                          //  5     6     7    8     9
char segundos;
char conteo;
char inicio;
char activar, j1, j2;
char cont1;
char cont2;
//--------------------------funciones-------------------------------------------
char indicador (char analizar);
//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void) {
    
    if(RBIF == 1){
        if (RB0 == 0){
            inicio++;
        }else if(RB1 == 0 && activar == 1){
            cont1++;
            if(cont1 == 10){
                j1++;
                cont1 = 0;
            }
            
        }else if(RB2 == 0 && activar == 1){
            cont2++;
            if(cont2 == 10){
                j2++;
                cont2 = 0;
            }
        }
        RBIF = 0;
    }
//    
//    else if(T0IF == 1){
//        conteo++;
//        
//        if(conteo == 4){
//            segundos++;
//            if (segundos == 255){
//                segundos = 0;
//            }
//        }
//        
//        T0IF = 0;
//        TMR0 = 60;                  //Reinicio del timmer (25ms)
//    }
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    ANSEL = 0X00;
    ANSELH = 0X00;     // solo pines digitales
    
    TRISA = 0x00;    
    TRISB = 0b00000111;    
    TRISC = 0x00;    
    TRISD = 0x00;    
    TRISE = 0x00;    
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                            //Config. PULL-UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
                        //Config. interrupcion RB
    IOCBbits.IOCB0 = 1; // pines en lo que habra interrupcion por cambio 
    IOCBbits.IOCB1 = 1; //de estado
    IOCBbits.IOCB2 = 1;
    RBIF = 0;
    
//                                //Config. timmer0 
//    OPTION_REGbits.T0CS = 0;    //Uso reloj interno
//    OPTION_REGbits.PSA = 0;     //Uso pre-escaler
//    OPTION_REGbits.PS = 0b111;  //PS = 111 / 1:256
//    TMR0 = 60;                  //Reinicio del timmer (25ms)
//    
    
    INTCONbits.GIE = 1;     //habilito interrupciones
    INTCONbits.RBIE = 1;    //activo interrupciones por cambio de estado
    INTCONbits.RBIF = 0;    //bajo la bandera
//    INTCONbits.T0IE = 1;    //activo interrupciones por timmer 0
//    INTCONbits.T0IF = 0;    //bajo la bandera
    
    segundos = 0;           //valor incial variable
    conteo = 0;
    inicio = 0;
    activar = 0;
    j1 = 0;
    j2 = 0;
    
    PORTA = 0x00;           //Estado inicial
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
       if (inicio == 1){
           RB7 = 1;         //poner en rojo el semaforo
           RB6 = 0;
           RB5 = 0;
           PORTA = num_display[3];
           __delay_ms(1000);
           PORTA = num_display[2];
           __delay_ms(1000);
           
           RB7 = 0;         //poner en amarillo el semaforo
           RB6 = 1;
           RB5 = 0;
           PORTA = num_display[1];
           __delay_ms(600);
           
           RB7 = 0;         //poner en verde el semaforo
           RB6 = 0;
           RB5 = 1;
           PORTA = num_display[0];
           __delay_ms(1000);
           
           RB7 = 0;         //apagar
           RB6 = 0;
           RB5 = 0;
           PORTA = 0x00;
           inicio++;
           activar = 1;
       } 
       if(activar == 1){
           PORTC = ~indicador (j1);
           PORTD = ~indicador (j2);
       }
       if (j1 == 8){
           PORTA = num_display[1];
           RE0 = 1;
           RE1 = 0;
           __delay_ms(2000);
           inicio = 0;
           activar = 0;
           j1 = 0;
           j2 = 0;
           PORTA = 0;
           PORTE = 0X00;
           PORTC = 0X00;
           PORTD = 0X00;
       }else if(j2 == 8){
           PORTA = num_display[2];
           RE0 = 0;
           RE1 = 1;
           __delay_ms(2000);
           inicio = 0;
           activar = 0;
           j1 = 0;
           j2 = 0;
           PORTA = 0;
           PORTE = 0X00;
           PORTC = 0X00;
           PORTD = 0X00;
       }
    }
    return;
}


char indicador (char analizar){
    char salida;
    switch(analizar){
        case(0):
            salida = 0b00000000;                   
            break;
        case(1):
            salida = 0b00000001;                   
            break;
        case(2):
            salida = 0b00000011;                   
            break;
        case(3):
            salida = 0b00000111;                   
            break;
        case(4):
            salida = 0b00001111;                   
            break;
        case(5):
            salida = 0b00011111;                   
            break;
        case(6):
            salida = 0b00111111;                   
            break;
        case(7):
            salida = 0b01111111;                   
            break;
        case(8):
            salida = 0xFF;                   
            break;
    }
    return salida;
}