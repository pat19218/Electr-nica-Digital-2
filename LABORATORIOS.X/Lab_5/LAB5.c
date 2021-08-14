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
 * Last modification 13 de agosto 2021, 24:00
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

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
char centena, decena, unidad;
char old;
char ingreso, pos, total;
char entrante [2];

//--------------------------funciones-------------------------------------------
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    if(RBIF == 1){
        if (RB0 == 0){
            PORTD++;
        }else if(RB1 == 0){
            PORTD--;
        }
        old = (RB0 != 0 && RB1 != 0) ? 1:0; 
        RBIF = 0;
    }
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    ANSEL = 0x00;
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0x00;
    TRISB = 0x03;
    TRISC = 0x00;
    TRISD = 0x00;
    
    USART_Init();
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
    old = 1;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    
    //------------------------------loop principal----------------------------------
    while (1){
        
        centena = centenas(PORTD);
        decena = decenas(PORTD);
        unidad = unidades(PORTD);
        centena += 48;
        decena += 48;
        unidad += 48;
        
        if(old || total != PORTA){
            USART_Cadena("\n\r\n\r");
            USART_Cadena("El valor del puerto D es: ");
            USART_Tx(centena);
            USART_Tx(decena);
            USART_Tx(unidad);
            USART_Cadena("\n\r");
            centena = centenas(PORTA);
            decena = decenas(PORTA);
            unidad = unidades(PORTA);
            centena += 48;
            decena += 48;
            unidad += 48;
            USART_Cadena("El valor del puerto D es: ");
            USART_Tx(centena);
            USART_Tx(decena);
            USART_Tx(unidad);
            USART_Cadena("\n\r\n\r");
            old = 0x00;
        }
        
//         if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
//            ingreso = USART_Rx();
//            if(ingreso > 47 && ingreso < 58){
//                entrante[pos] = ingreso;
//                pos++;
//                //PORTD++;
//                if (pos > 2){
//                    pos = 0;
//                    total = (entrante[0] - 48) * 100;
//                    total +=(entrante[1] - 48) *10;
//                    total +=(entrante[2] - 48);
//                    PORTA = total;
//                    //PORTD++;
//                }
//            }
//       }
       
        if(ingreso == '+'){
            PORTD++;
        }else if(ingreso == '-'){
            PORTD--;
        }
        
        ingreso = 0;
        
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
