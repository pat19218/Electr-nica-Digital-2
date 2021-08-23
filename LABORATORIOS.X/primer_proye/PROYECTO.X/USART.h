/* 
 * File:   
 * Author: Cristhofer Patzán
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <pic16f887.h>
// 

//VARIABLES


//FUNCIONES
void USART_Init(void);
void USART_Tx(char data);
char USART_Rx(void);
void USART_Cadena(char *str);
void USART_volt(char cen, char dec, char uni);

//MACRO
 //#define tmr0_value 60


#endif	/* UART_H */

