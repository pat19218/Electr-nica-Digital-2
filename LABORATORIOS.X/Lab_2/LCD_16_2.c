#include <xc.h>
#include <stdint.h>
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
//    LCD_PORT = 0b00110000;      //primer dato a enviar
//    RS_PIN = 0;                 //modo comando
//    RW_PIN = 0;                 //modo escritura
//    __delay_us(10);
//    E_PIN = 1;                  //habilito el LCD
//    __delay_us(10);
//    E_PIN = 0;                  //deshabilito el LCD
    
    __delay_ms(5);              //segun datasheet
    
    LCD_8_comando(0b00110000);
//    LCD_PORT = 0b00110000;      //segundo dato a enviar
//    RS_PIN = 0;                 //modo comando
//    RW_PIN = 0;                 //modo escritura
//    __delay_us(10);
//    E_PIN = 1;                  //habilito el LCD
//    __delay_us(10);
//    E_PIN = 0;                  //deshabilito el LCD
    
    __delay_us(100);
    
    LCD_8_comando(0b00110000);
//    LCD_PORT = 0b00110000;      //tercer dato a enviar
//    RS_PIN = 0;                 //modo comando
//    RW_PIN = 0;                 //modo escritura
//    __delay_us(10);
//    E_PIN = 1;                  //habilito el LCD
//    __delay_us(10);
//    E_PIN = 0;                  //deshabilito el LCD
    
    __delay_us(100);            //darle tiempo produnte
    
    //8 bits configuración
    
    LCD_8_comando(0b00111000);
//    RS_PIN = 0;
//    RW_PIN = 0;                 //PARA 2 LINEAS N=1
//    LCD_PORT = 0b00111000;      //F=0 SI O SI TENDRA QUE SER DE 8DOTS
    __delay_us(100);
    
    LCD_8_comando(0b00001000);
//    RS_PIN = 0;
//    RW_PIN = 0;                 
//    LCD_PORT = 0b00001000;      
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
    __delay_us(20);
    E_PIN = 1;                  //habilito el LCD
    __delay_us(20);
    E_PIN = 0;                  //deshabilito el LCD
    
    return;
}

void LCD_XY(unsigned char x,unsigned char y){
    if(x>0){
        LCD_8_comando(0xC0 + y);
    }
    else{
        LCD_8_comando(0x80 + y);
    }
}

void LCD_Cadena(const char *dato){
    while(*dato){
        __delay_us(100);
        LCD_Data(*dato);
        dato++;
    }
}

void LCD_Data(char data){
    __delay_us(100);
    
    LCD_PORT = data;
    //Configuración
    RW_PIN = 0;     //Modo Escritura
    RS_PIN = 1;     //Modo Caracter
    __delay_us(20); 
    E_PIN = 1;      //Habilita LCD
    __delay_us(20);
    E_PIN = 0;      //Deshabilita LCD
    
    return;
}