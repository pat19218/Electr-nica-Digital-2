/*
   Autor: Cristhofer Patzán
    Descripcion: juego para 2 personas, gana el que primero
                 presione 8 veces su push button el conteo se
                realiza por un contador de decadas
   Materiales:  TivaC LaunchPad TM4C123GXL, 16 leds, 16 res
   Fecha de creacion: 13 de  septiembre 2021, 17:45
   Ultima modificacion: 13 de septiembre 2021,
*/

#define rojo RED_LED    //pin 30 PF_1
#define verde GREEN_LED //pin 39 PF_3
#define azul BLUE_LED   //pin 40 PF_2
#define sw1 PUSH1       //pin 31 PF_4
#define sw2 PUSH2       //pin 17 PF_0

byte estado1 = 0;   //lectura del estado de los switches
byte estado2 = 0;
byte old_estado1 = 0;   //anti-rebote
byte old_estado2 = 0;
byte punteo1 = 0;   //punteos de cada jugador
byte punteo2 = 0;
bool juega = 0;     //recordar que
//true = 1 = HIGH = +-(x)
//false = 0 = LOW


void setup() {
  pinMode(sw1, INPUT_PULLUP); //config. Entrada en Pull-up
  pinMode(sw2, INPUT_PULLUP); //estado sin presionar 'HIGH'

  pinMode(verde, OUTPUT);     //los configuro como pines de salida
  pinMode(rojo, OUTPUT);
  pinMode(azul, OUTPUT);
}

void loop() {
  old_estado1 = estado1;
  old_estado2 = estado2;
  estado1 = digitalRead(sw1);
  estado2 = digitalRead(sw2);

  if ((estado1 == 0 || estado2 == 0) && juega == 0) { //cualquiera presionado
    juega = 1;
    digitalWrite(rojo, 1);
    digitalWrite(verde, 0);
    digitalWrite(azul, 0);
    delay(1500);
    digitalWrite(rojo, 1);
    digitalWrite(verde, 1);
    digitalWrite(azul, 0);
    delay(1500);
    digitalWrite(rojo, 0);
    digitalWrite(verde, 1);
    digitalWrite(azul, 0);
    delay(1500);
    digitalWrite(rojo, 0);
    digitalWrite(verde, 0);
    digitalWrite(azul, 0);
  }

  if (juega) {
    if (estado1 == 0 && estado2 == 1) { //sw1 presionado, suma
      if (estado1 != old_estado1) {
        punteo1++;
        digitalWrite(rojo, 1);
        digitalWrite(verde, 0);
      }
    }
    else if (estado1 == 1 && estado2 == 0) { //sw2 presionado, suma
      if (estado2 != old_estado2) {
        punteo2++;
        digitalWrite(rojo, 0);
        digitalWrite(verde, 1);
      }
    }

    if ((punteo1 > 8) || (punteo2 > 8) ) {
      digitalWrite(rojo, 1);
      digitalWrite(verde, 0);
      digitalWrite(azul, 1);
      delay(2000);

      if (punteo1 >= 8) {
        digitalWrite(rojo, 1);
        digitalWrite(verde, 0);
        digitalWrite(azul, 0);
      } else {
        digitalWrite(rojo, 0);
        digitalWrite(verde, 1);
        digitalWrite(azul, 0);
      }
      punteo1 = 0;  //reinicio parametros
      punteo2 = 0;
      juega = 0;
      delay(1300);
      digitalWrite(rojo, 0);
      digitalWrite(verde, 0);
      digitalWrite(azul, 0);
    }
  }
}
