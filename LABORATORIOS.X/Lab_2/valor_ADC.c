#include <xc.h>
#include <stdint.h>
#include "valor_ADC.h"

char hex_mayor (char in){
    data_mayor = in % 16;
}

char hex_menor (char in){
    data_menor = (in/16) % 16;
}
