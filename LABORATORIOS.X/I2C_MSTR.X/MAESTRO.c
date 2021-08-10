/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de uso de I2C Master
 * Created on 17 de febrero de 2020, 10:32 AM
 */
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD_16_2.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);
int voltaje1, voltaje2, voltaje3;
char centena1, decena1, unidad1;
char centena2, decena2, unidad2;
char centena3, decena3, unidad3;
double termo;
uint16_t temp;


//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        PORTA = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
          
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        PORTB = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
          
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        temp = ((I2C_Master_Read(0))<<8);
        temp += I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        temp &= ~0x003;
        termo = ((175.72*temp)/65536) - 46.85;
        if(termo < 0){
            termo *= -1;
            Lcd_Set_Cursor(2,11);
            Lcd_Write_Char('-');            
        }else{
            Lcd_Set_Cursor(2,11);
            Lcd_Write_Char(' ');            
        }            
        
        voltaje1 = PORTA * 1.9607;
        voltaje2 = PORTB * 1.9607;
        
        
        centena1 = (centenas(voltaje1)) + 48;
        centena2 = (centenas(voltaje2))+ 48;
        centena3 = centenas(termo) + 48;
        
        decena1 = (decenas(voltaje1))+ 48;
        decena2 = (decenas(voltaje2))+ 48;
        decena3 = decenas(termo) + 48;
        
        unidad1 = (unidades(voltaje1))+ 48;
        unidad2 = (unidades(voltaje2))+ 48;
        unidad3 = unidades(termo) + 48;
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1    S2     S3");
        
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(centena1);
        Lcd_Write_Char('.');
        Lcd_Write_Char(decena1);
        Lcd_Write_Char(unidad1);
        
        Lcd_Set_Cursor(2,6);
        Lcd_Write_Char(centena2);
        Lcd_Write_Char('.');
        Lcd_Write_Char(decena2);
        Lcd_Write_Char(unidad2);
        
        Lcd_Set_Cursor(2,12);
        Lcd_Write_Char(centena3);
        Lcd_Write_Char(decena3);
        Lcd_Write_Char(unidad3);
        Lcd_Write_Char(0b11011111); //0xDF = 223 = °

    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    Lcd_Init();
    OSCCONbits.IRCF = 0b111; //Config. de oscilacion 8MHz
    OSCCONbits.SCS = 1;      //reloj interno    
    I2C_Master_Init(100000); // Inicializar Comuncación I2C
       
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