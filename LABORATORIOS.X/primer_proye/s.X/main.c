/* File:   PROYECTO.c
 * Dispositivo:	PIC16f887
 * Author: isaac
 *
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa: Pic y IO esclavo
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
#include "I2C.h"

//--------------------------directivas del compilador---------------------------
#define _XTAL_FREQ 8000000 //__delay_ms(x)

//---------------------------variables------------------------------------------
char z;

char DataBuffer[6];

uint32_t Raw_humedad;
int humedad;

//char entero, decimal;


//--------------------------funciones-------------------------------------------
void Init_AHT10 (void);

//---------------------------interrupciones-------------------------------------

void __interrupt()isr(void){
    
}

//----------------------configuracion microprocesador---------------------------

void main(void) {
    ANSEL = 0x00;
    ANSELH = 0x00;      // solo pines digitales
    
    TRISA = 0x00;
    
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno
    
                           //Estado inicial
    
    I2C_Master_Init(100000); // Inicializar Comuncación I2C
    Init_AHT10();
    
    PORTA = 0x00;
    
    //------------------------------loop principal----------------------------------
    while (1){
        
        //inicio de medición del sensor
        I2C_Master_Start();
        I2C_Master_Write(0x38); //inicio comunicación   addres can be 0x70
        I2C_Master_Write(0xAC); //comando de incio de medicion
        I2C_Master_Write(0x33); //primer dato (humedad)
        I2C_Master_Write(0x00); //primer dato (temperatura)
        I2C_Master_Stop();
        __delay_ms(80);         //de almenos 75mS
        
        //lectura en loop principal
        I2C_Master_Start();
        I2C_Master_Write(0x39); //ubicacion para lectura
        DataBuffer[0] = I2C_Master_Read(0); //humedad
        DataBuffer[1] = I2C_Master_Read(0); //humedad
        DataBuffer[2] = I2C_Master_Read(0); //humedad y temperatura
        DataBuffer[3] = I2C_Master_Read(0); //temperatura
        DataBuffer[4] = I2C_Master_Read(0); //temperatura
        DataBuffer[5] = I2C_Master_Read(0); //temperatura
        I2C_Master_Stop();
        __delay_ms(200);
        
        Raw_humedad = (((uint32_t)DataBuffer[1]<<16) | ((uint16_t)DataBuffer[2]<<8) | (DataBuffer[3]))>>4; //20 bits de datos
        humedad = (char)(Raw_humedad * 0.000095);   //parte entera
        
        //aseguro rango de humedad y esta con una presicion del 2%
        if(humedad < 0){humedad = 0;}
        if(humedad > 100){humedad = 100;}
        
//        entero = (char)temperatura;
//        decimal = (char)((temperatura - entero)*10);        
//        
//        
//         cen = centenas(temperatura) + 48;     //caracter de temperatura
//         dec = decenas(temperatura) + 48;         
//         uni = unidades(temperatura) + 48;         
         
        PORTA = humedad;
        
        __delay_ms(200);
        
        
    }
    return;
}

void Init_AHT10 (void){
    //0x38 ubicacion del sensor  0x39 lectura del sensor
    __delay_ms(40); //delay para que se inicialice el sensor
    //soft reset
    I2C_Master_Start();
    I2C_Master_Write(0x38); //inicio comunicación  addres can be 0x70
    I2C_Master_Write(0xBA); //comando de reinicio suave
    I2C_Master_Stop();
    __delay_ms(20);

    //Init medidas
    I2C_Master_Start();
    I2C_Master_Write(0x38); //inicio comunicación   addres can be 0x70
    I2C_Master_Write(0xE1); //comando de inicio
    I2C_Master_Write(0xAC); //comando de inicio de medicion 
    I2C_Master_Stop();
    __delay_ms(350);
}
