
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
void LCD_8_comando(unsigned char dato); //dentro de la config
void LCD_8XY(unsigned char x,unsigned char y);  //dar ubicacion 8 con pines
void LCD_8Cadena(const char *dato); //mandar mensaje con 8 pines
void LCD_8Data(char data);  //mandar dato con 8 pines
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
//Macros
#define LCD_PORT PORTD
#define LCD_TRIS TRISD

#define RS_PIN PORTCbits.RC0
#define RS_TRIS TRISCbits.TRISC0

#define RW_PIN PORTCbits.RC1
#define RW_TRIS TRISCbits.TRISC1

#define E_PIN  PORTCbits.RC2
#define E_TRIS TRISCbits.TRISC2

#define RS PORTDbits.RD2
#define EN RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7


#endif	/* LCD_16_2_H */

