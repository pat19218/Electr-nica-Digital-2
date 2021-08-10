#include <xc.h>
#include "valor_ADC.h"

char hex_mayor (char in){
    data_mayor = in % 16;
    return data_mayor;
}

char hex_menor (char in){
    data_menor = (in/16) % 16;
    return data_menor;
}
