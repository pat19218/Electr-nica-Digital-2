
/* 
 * File: libreria para lcd
 * Author: Cristhofer Patzán
 * Comments:
 * Revision history: 25/07/2021
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_16_2_H
#define	LCD_16_2_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>


//variables


//Funciones
void LCD_Init8(void);       //llamar para la config
void LCD_Init4(void);
void LCD_8_comando(unsigned char dato); //dentro de la config
void LCD_4Comando(unsigned char cmd);   //dentro de la config
void LCD_8XY(unsigned char x,unsigned char y);  //dar ubicacion 8 con pines
void LCD_4XY(unsigned char x,unsigned char y);  //dar ubicacion con 4 pines
void LCD_8Cadena(const char *dato); //mandar mensaje con 8 pines
void LCD_4Cadena(const char *dato); //mandar mensaje con 4 pines
void LCD_8Data(char data);  //mandar dato con 8 pines
void LCD_4Data(char data);  //mandar dato con 4 pines

//Macros
#define LCD_PORT PORTD
#define LCD_TRIS TRISD

#define RS_PIN PORTCbits.RC0
#define RS_TRIS TRISCbits.TRISC0

#define RW_PIN PORTCbits.RC1
#define RW_TRIS TRISCbits.TRISC1

#define E_PIN  PORTCbits.RC2
#define E_TRIS TRISCbits.TRISC2

#define _XTAL_FREQ 8000000 //__delay_ms(x)

#endif	/* LCD_16_2_H */

