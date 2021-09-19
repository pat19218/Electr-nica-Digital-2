/*
   Autor: Cristhofer Patzán
    Descripcion: juego para 2 personas, gana el que primero
                 presione 8 veces su push button el conteo se
                realiza por un contador de decadas
   Materiales:  TivaC LaunchPad TM4C123GXL, 16 leds, 16 res
   Fecha de creacion: 13 de  septiembre 2021, 17:45
   Ultima modificacion: 13 de septiembre 2021,20:09
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

  pinMode(23, OUTPUT);  //j11
  pinMode(24, OUTPUT);  //j12
  pinMode(25, OUTPUT);  //j13
  pinMode(26, OUTPUT);  //j14
  pinMode(27, OUTPUT);  //j15
  pinMode(28, OUTPUT);  //j16
  pinMode(29, OUTPUT);  //j17
  pinMode(10, OUTPUT);  //j18

  pinMode(2, OUTPUT);  //j21
  pinMode(3, OUTPUT);  //j22
  pinMode(4, OUTPUT);  //j23
  pinMode(5, OUTPUT);  //j24
  pinMode(6, OUTPUT);  //j25
  pinMode(7, OUTPUT);  //j26
  pinMode(8, OUTPUT);  //j27
  pinMode(9, OUTPUT);  //j28

  Serial.begin(9600);
  delay(1);
  Serial.println("Presiona cualquier boton para iniciar el juego");
  Serial.println("ganara el primero en presionar 8 veces \n");
}

void loop() {
  old_estado1 = estado1;
  old_estado2 = estado2;
  estado1 = digitalRead(sw1);
  estado2 = digitalRead(sw2);

  if ((estado1 == 0 || estado2 == 0) && juega == 0) { //cualquiera presionado
    juega = 1;
    Serial.println("Empieza en: 3");
    digitalWrite(rojo, 1);
    digitalWrite(verde, 0);
    digitalWrite(azul, 0);
    delay(1500);
    Serial.println("Empieza en: 2");
    digitalWrite(rojo, 1);
    digitalWrite(verde, 1);
    digitalWrite(azul, 0);
    delay(1500);
    Serial.println("Empieza en: 1");
    digitalWrite(rojo, 0);
    digitalWrite(verde, 1);
    digitalWrite(azul, 0);
    delay(1500);
    Serial.println("  Buena suerte  ");
    digitalWrite(rojo, 0);
    digitalWrite(verde, 0);
    digitalWrite(azul, 0);
    estado1 = 0;
    estado2 = 0;
  }

  if (juega) {
    if (estado1 == 0 && estado2 == 1) { //sw1 presionado, suma
      if (estado1 != old_estado1) {
        punteo1++;

        Serial.print("Juagador 1 = ");
        Serial.print(punteo1);
        Serial.print("  Juagador 2 = ");
        Serial.println(punteo2);

        digitalWrite(rojo, 1);
        digitalWrite(verde, 0);
      }
    }
    else if (estado1 == 1 && estado2 == 0) { //sw2 presionado, suma
      if (estado2 != old_estado2) {
        punteo2++;

        Serial.print("Juagador 1 = ");
        Serial.print(punteo1);
        Serial.print("  Juagador 2 = ");
        Serial.println(punteo2);

        digitalWrite(rojo, 0);
        digitalWrite(verde, 1);
      }
    }
    switch (punteo1) {
      case 1:
        digitalWrite(23, 1);
        digitalWrite(24, 0);
        digitalWrite(25, 0);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 2:
        digitalWrite(23, 0);
        digitalWrite(24, 1);
        digitalWrite(25, 0);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 3:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 1);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 4:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 0);
        digitalWrite(26, 1);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 5:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 0);
        digitalWrite(26, 0);
        digitalWrite(27, 1);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 6:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 0);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 1);
        digitalWrite(29, 0);
        digitalWrite(10, 0);
        break;
      case 7:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 0);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 1);
        digitalWrite(10, 0);
        break;
      case 8:
        digitalWrite(23, 0);
        digitalWrite(24, 0);
        digitalWrite(25, 1);
        digitalWrite(26, 0);
        digitalWrite(27, 0);
        digitalWrite(28, 0);
        digitalWrite(29, 0);
        digitalWrite(10, 1);
        break;
    }
    switch (punteo2) {
      case 1:
        digitalWrite(2, 1);
        digitalWrite(3, 0);
        digitalWrite(4, 0);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 2:
        digitalWrite(2, 0);
        digitalWrite(3, 1);
        digitalWrite(4, 0);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 3:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 1);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 4:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 0);
        digitalWrite(5, 1);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 5:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 0);
        digitalWrite(5, 0);
        digitalWrite(6, 1);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 6:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 0);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 1);
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        break;
      case 7:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 0);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 1);
        digitalWrite(9, 0);
        break;
      case 8:
        digitalWrite(2, 0);
        digitalWrite(3, 0);
        digitalWrite(4, 1);
        digitalWrite(5, 0);
        digitalWrite(6, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);
        digitalWrite(9, 1);
        break;
    }


    if ((punteo1 >= 8) || (punteo2 >= 8) ) {
      digitalWrite(rojo, 1);
      digitalWrite(verde, 0);
      digitalWrite(azul, 1);
      delay(2000);

      if (punteo1 >= 8) {
        digitalWrite(rojo, 1);
        digitalWrite(verde, 0);
        digitalWrite(azul, 0);
        Serial.println(" Juagador 1 es el GANADOR \n");
      } else {
        Serial.println(" Juagador 2 es el GANADOR \n");
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

      Serial.println("Presiona cualquier boton para iniciar el juego");
      Serial.println("ganara el primero en presionar 8 veces \n");
    }
  }
}
