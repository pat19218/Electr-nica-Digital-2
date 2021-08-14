#include <xc.h>
#include "USART.h"

void USART_Init(void){
                                //Confi. serial comunication
    TXSTAbits.SYNC = 0;     //asincrono
    TXSTAbits.BRGH = 1;     //high speed
    BAUDCTLbits.BRG16 = 1;  //uso los 16 bits
   
    SPBRG = 207;   //revisar tabla BAUD RATES FOR ASYNCHRONOUS MODES (CONTINUED)                      
    SPBRGH = 0;    //pagina 168 del datasheet del 2009         
    
    RCSTAbits.SPEN = 1;     //enciendo el modulo
    RCSTAbits.RX9 = 0;      //No trabajo a 9 bits
    RCSTAbits.CREN = 1;     //activo recepción
    TXSTAbits.TXEN = 1;     //activo transmision 
}


void USART_Tx(char data){       //envio de un caracter
    while(TXSTAbits.TRMT == 0);
    TXREG = data;
}

char USART_Rx(){                //Lectura de comunicacion serial
    return RCREG; 
   }

void USART_Cadena(char *str){   //Envio de cadena de caracteres
    while(*str != '\0'){
        USART_Tx(*str);
        str++;
    }
}

void USART_volt(char cen, char dec, char uni){
    cen += 48;
    dec += 48;
    uni += 48;
    
    USART_Tx(cen);
    USART_Tx('.');
    USART_Tx(dec);
    USART_Tx(uni);
    USART_Tx('V');
    return;
}


