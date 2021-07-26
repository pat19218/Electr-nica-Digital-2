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


void LCD_Init4(void){
    //4bits -> RD0 RD3
    LCD_PORT = LCD_PORT & 0xf0;
    LCD_TRIS &= 0xf0; //LCD_TRIS = LCD_TRIS & 0xf0;
    
    RW_TRIS = 0;
    RS_TRIS = 0;
    E_TRIS = 0;
    
    RW_PIN = 0;
    RS_PIN = 0;
    E_PIN = 0;
    
    __delay_ms(15); //Segun datasheet

    LCD_4Comando(0x30);
    __delay_ms(5);
    LCD_4Comando(0x30);
    __delay_us(100);
    LCD_4Comando(0x32);
    __delay_us(100);
    
    //4 bits configuracion:
    __delay_us(100);
    LCD_4Comando(0x28);      //Lineas y caracteres
    __delay_us(100);
    LCD_4Comando(0x08);      //Display cursor y parpadero: apagado
    __delay_us(100);
    LCD_4Comando(0x0f);      //Display cursor y parpadero: enciende
    __delay_us(100);
    LCD_4Comando(0x01);      //Limpiar LCD
    __delay_us(100);
    LCD_4Comando(0x04);
    __delay_us(100);
    
    __delay_us(100);
    LCD_4Comando(0x06);      //Display
    LCD_4Comando(0x0C);      //Incremento y no se mueve
    return;
}

void LCD_4Comando(unsigned char cmd){
    //30 = 0011 0000 
    LCD_PORT &= 0xf0;
    LCD_TRIS &= 0xf0;
    
    //30 -> + significativo 3
    LCD_PORT = LCD_PORT | ((cmd>>4)&0x0f);
    /*      30= 0011 0000
     *     >>4= 0000 0011
     *     &0f= 0000 1111
     *        =  0   0011   
     */ 
    
    //Configuración
    RW_PIN = 0;     //Modo Escritura
    RS_PIN = 0;     //Modo Comando
    __delay_us(10);  
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    //30 -> - significativo 0
    LCD_PORT &= 0xf0;
    LCD_PORT |= cmd&0x0f;
    __delay_us(10);  
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    LCD_TRIS |= 0x0f;
    
    return;
}

void LCD_4XY(unsigned char x,unsigned char y){
    if(x>0){
        LCD_4Comando(0xC0+y);
    }
    else{
        LCD_4Comando(0x80+y);
    }
}

void LCD_4Cadena(const char *dato){
    while(*dato){
        __delay_us(100);
        LCD_4Data(*dato);
        dato++;
    }
}

void LCD_4Data(char data){
    __delay_us(100);
    LCD_PORT &= 0xf0;
    LCD_TRIS &= 0xf0;
    
    LCD_PORT = LCD_PORT | ((data>>4)&0x0f);
    //Configuración
    RW_PIN = 0;     //Modo Escritura
    RS_PIN = 1;     //Modo Caracter
    __delay_us(10);  //18TCY -> 20MHZ: 18*TCY = 18*(4/20MHZ) = 3.6uS
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    LCD_PORT &= 0xf0;
    LCD_PORT |= data&0x0f;
    
    __delay_us(10);  //18TCY -> 20MHZ: 18*TCY = 18*(4/20MHZ) = 3.6uS
    E_PIN = 1;      //Habilita LCD
    __delay_us(10);
    E_PIN = 0;      //Deshabilita LCD
    
    LCD_TRIS |= 0x0f;
    
    return;
    
}

