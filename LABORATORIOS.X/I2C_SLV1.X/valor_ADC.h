/* 
 * File:   
 * Author: Cristhofer Patzán
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef VALOR_ADC_H
#define	VALOR_ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//VARIABLES
unsigned char data_mayor;
unsigned char data_menor;

//FUNCIONES
char hex_mayor (char in);
char hex_menor (char in);

//MACRO
 //#define tmr0_value 60

#endif	/* VALOR_ADC_H */

