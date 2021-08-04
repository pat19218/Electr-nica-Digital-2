/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Valores de ADC mediante SPI "MAESTRO"
 * Hardware: Pic 16f887, leds, res, comunicación SPI y USART
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
#include "USART.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
char datoIn1, datoIn2;     //datos donde se almacenan 
int voltaje1, voltaje2, voltaje3;
char centena1, decena1, unidad1;
char centena2, decena2, unidad2;
char centena3, decena3, unidad3;
char ok;
char old1, old2;
char ingreso;
char entrante [2];
char pos;
int total;
//--------------------------funciones-------------------------------------------
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);

//---------------------------interrupciones-------------------------------------
void __interrupt()isr(void){
   
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    USART_Init();
    ANSEL = 0x00; 
    ANSELH = 0x00;     // solo pines digitales
    
    TRISA = 0x00; //todo salida
    TRISB = 0x00; //todo salida
    TRISD = 0x00; //todo salida
    TRISC = 0b10010000; //M_SDI y RX entradas el resto salidas
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
    
                           //Estado inicial
    old1 = 0;
    old2 = 0;
    pos = 0;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
    PORTC = 0b0000000100;
    
    //------------------------------loop principal----------------------------------
    while(1){
        old1 = datoIn1;
        old2 = datoIn2;
        
        RC2 = 0;    //selecciono slave
        __delay_ms(1);
        spiWrite(0x00);         //indico que dato quiero recibir
        datoIn2 = spiRead();    //leo dato recibido
        __delay_ms(1);
        RC2 = 1;    //deselecciono slave
        __delay_ms(1);
        
        RC2 = 0;    //selecciono slave
        __delay_ms(1);
        spiWrite(0x01);         //indico que dato quiero recibir
        datoIn1 = spiRead();    //leo dato recibido
         __delay_ms(1);
        RC2 = 1;    //deselecciono slave
        __delay_ms(1);
        
        voltaje1 = datoIn2 * 1.96;
        voltaje2 = datoIn1 * 1.96;
        voltaje3 = PORTD * 1.96;
        
        centena1 = centenas(voltaje1);
        centena2 = centenas(voltaje2);
        centena3 = centenas(voltaje3);
        
        decena1 = decenas(voltaje1);
        decena2 = decenas(voltaje2);
        decena3 = decenas(voltaje3);
        
        unidad1 = unidades(voltaje1);
        unidad2 = unidades(voltaje2);
        unidad3 = unidades(voltaje3);
        
        
        if(old1 != datoIn1 || old2 != datoIn2 || ok != PORTD){
            USART_Cadena("\n\r\n\rpresiona + para incrementar \n\r");
            USART_Cadena("presiona - para decrementar ");
            USART_Cadena("\n\r\n\rVoltaje Pot 1: ");
            USART_volt(centena1, decena1, unidad1);
            USART_Cadena("\n\rVoltaje Pot 2: ");
            USART_volt(centena2, decena2, unidad2);
            USART_Cadena("\n\rVoltaje PORTD: ");
            USART_volt(centena3, decena3, unidad3);
            USART_Cadena("\n\r\n\r");
        }
        
        ok = PORTD;        
        if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
            ingreso = USART_Rx();
            if(ingreso > 47 && ingreso < 58){
                entrante[pos] = ingreso;
                pos++;
                //PORTD++;
                if (pos > 2){
                    pos = 0;
                    total = (entrante[0] - 48) * 100;
                    total +=(entrante[1] - 48) *10;
                    total +=(entrante[2] - 48);
                    PORTD = total/1.958;
                    //PORTD++;
                }
            }
       }
       
        if(ingreso == '+'){
            PORTD++;
        }else if(ingreso == '-'){
            PORTD--;
        }
        
        ingreso = 0;
        
        PORTA = datoIn1;
        PORTB = datoIn2;
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