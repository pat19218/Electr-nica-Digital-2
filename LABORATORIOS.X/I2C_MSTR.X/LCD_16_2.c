#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "LCD_16_2.h"
#define _XTAL_FREQ 8000000 //__delay_ms(x)

void LCD_Init8(void){
    LCD_PORT = LCD_PORT & 0x00; //limpiar el puerto
    LCD_TRIS = 0x00;            //SALIDAS
    RS_TRIS = 0;        //pines como salida
    RW_TRIS = 0;
    E_TRIS = 0;
    RS_PIN = 0;         //estado inicial
    RW_PIN = 0;
    E_PIN = 0;
        
    __delay_ms(16);     //segun datasheet
    
    LCD_8_comando(0b00110000);    
    __delay_ms(5);              //segun datasheet
    
    LCD_8_comando(0b00110000);
    __delay_us(100);
    
    LCD_8_comando(0b00110000);
    __delay_us(100);            //darle tiempo produnte
    
    //8 bits configuración
    
    LCD_8_comando(0b00111000);
    __delay_us(100);
    
    LCD_8_comando(0b00001000);
    __delay_us(100);
    
    LCD_8_comando(0b00000001);
    __delay_us(100);
    
    LCD_8_comando(0b00000100);     //Que se incremente y q no se mueva
    __delay_us(100);
    
    __delay_us(100);
    LCD_8_comando(0x06);           //display
    LCD_8_comando(0x0C);           //incremento y q no se mueva
    return;
}

void LCD_8_comando(unsigned char dato){
    LCD_PORT = dato;
    RS_PIN = 0;                 //modo comando
    RW_PIN = 0;                 //modo escritura
    __delay_us(10);
    E_PIN = 1;                  //habilito el LCD
    __delay_us(10);
    E_PIN = 0;                  //deshabilito el LCD
    
    return;
}

void LCD_8XY(unsigned char x,unsigned char y){
    if(x>0){
        LCD_8_comando(0xC0 + y);
    }
    else{
        LCD_8_comando(0x80 + y);
    }
}

void LCD_8Cadena(const char *dato){
    while(*dato){
        __delay_us(100);
        LCD_8Data(*dato);
        dato++;
    }
}

void LCD_8Data(char data){
    __delay_us(100);
    
    LCD_PORT = data;
    //Configuración
    RW_PIN = 0;     //Modo Escritura
    RS_PIN = 1;     //Modo Caracter
    __delay_us(10); 
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    return;
}

//**********************Para 4 bits********************************

void Lcd_Port(char a) {
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Init(void) {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(11);
    Lcd_Cmd(0x03);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a) {
    char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    Lcd_Port(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("LCD Library for");
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String("MPLAB XC8");
//    __delay_ms(2000);
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("Developed By");
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String("electroSome");
//    __delay_ms(2000);
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("www.electroSome.com");
//
//    for(a=0;a<15;a++)
//    {
//        __delay_ms(300);
//        Lcd_Shift_Left();
//    }
//
//    for(a=0;a<15;a++)
//    {
//        __delay_ms(300);
//        Lcd_Shift_Right();
//    }
//
//    Lcd_Clear();
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_Char('H');
//    Lcd_Write_Char('o');
//    Lcd_Write_Char('l');
//    Lcd_Write_Char('a');
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("Hola Mundo");
//    __delay_ms(2000);

