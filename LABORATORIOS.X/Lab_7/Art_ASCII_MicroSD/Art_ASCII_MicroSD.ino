/*
   Autor: Cristhofer Patzán
   Descripcion: Lector de SD, graficadaora de ART ASCII
   Materiales:  TivaC LaunchPad TM4C123GXL, SD y adaptador a SD
   Fecha de creacion: 26 de  septiembre 2021, 11:16
   Ultima modificacion: 26 de  septiembre 2021, 16:36
*/
#include <SPI.h>
#include <SD.h>

// Variables a usar dentro de la libreria
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 12; //cs PIN
String palabra;            //variable en la q ira la palabra

File dataFile;

void setup() {

  Serial.begin(9600);       //inicio comunicacion serial
  while (!Serial) {         //espero a q se establezca
    ;
  }
  SPI.setModule(0);         //Indespensable declarar el SPI a usar en TIVA C
  delay(2500);              //Tiempo para abrir la terminal serial
  Serial.print("\nInitializing SD card...\n");
  pinMode(12, OUTPUT);     // change this to 53 on a mega or 12 on a Tiva C


  if (!card.init(SPI_HALF_SPEED, chipSelect)) {                 //Lo inicio en configuración de obtencion de datos
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  if (!volume.init(card)) {       //Se inicia a corde a los formatos permitidos
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);              //accedo a la info
  root.ls(LS_R | LS_DATE | LS_SIZE);  //la imprimo en el orden de datos que quiero
  delay(1000);                        //espero a que aparezcan
  root.close();                       //cierro la configuracion de obtencion de datos


  if (!SD.begin(chipSelect)) {        //Inicio la comunicacion a la SD pero en config. de lectura/escritura
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  Serial.print("\n Escribe en minusculas el nombre del");
  Serial.println("documento txt a abrir");
}

void loop() {

  while (Serial.available()) {  //Confirmando que la comunicación Serial esté disponible para hacer la lectura del mensaje
    delay(10);                  //para dar estabilidad al programa
    char c = Serial.read();     //lee el caracter turno
    palabra += c;               //sumando el contenido de la variable c en la variable mensaje (inserta caracter a la variable)
  }

  if ((palabra == "mano\n") || (palabra == "MANO\n")){
    Serial.println();
    dataFile = SD.open("mano.txt");  //creo un espacio en la memoria para guardar la info del txt abierto para leer
    if (dataFile) {                           //si no esta vacio/existe el archivo imprimo lo leido
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();                       //cierro la comunicacion para que posterior a ello se pueda abrir para leer/escribir
      Serial.println();
      menu();
    }
    else {                                    //si no se puede abrir un archivo con el nombre declarado
      Serial.println("error opening mano.txt");
      menu();
    }

  } else if ((palabra == "guitarra\n")  || (palabra == "GUITARRA\n")){
    Serial.println();
    dataFile = SD.open("guitarra.txt");  //creo un espacio en la memoria para guardar la info del txt abierto para leer
    if (dataFile) {                           //si no esta vacio/existe el archivo imprimo lo leido
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();                       //cierro la comunicacion para que posterior a ello se pueda abrir para leer/escribir
      Serial.println();
      menu();
    }
    else {                                    //si no se puede abrir un archivo con el nombre declarado
      Serial.println("error opening guitarra.txt");
      menu();
    }

  } else if ((palabra == "logo\n")  || (palabra == "LOGO\n")){
    Serial.println();
    dataFile = SD.open("logo.txt");  //creo un espacio en la memoria para guardar la info del txt abierto para leer
    if (dataFile) {                           //si no esta vacio/existe el archivo imprimo lo leido
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();                       //cierro la comunicacion para que posterior a ello se pueda abrir para leer/escribir
      Serial.println();
      menu();
    }
    else {                                    //si no se puede abrir un archivo con el nombre declarado
      Serial.println("error opening logo.txt");
      menu();
    }

  } else if ((palabra == "hi\n")  || (palabra == "HI\n")){
    Serial.println();
    dataFile = SD.open("hi.txt");  //creo un espacio en la memoria para guardar la info del txt abierto para leer
    if (dataFile) {                           //si no esta vacio/existe el archivo imprimo lo leido
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();                       //cierro la comunicacion para que posterior a ello se pueda abrir para leer/escribir
      Serial.println();
      menu();
    }
    else {                                    //si no se puede abrir un archivo con el nombre declarado
      Serial.println("error opening hi.txt");
      menu();
    }

  } else if (palabra != "") {
    Serial.println("\n*************No es opcion valida*************");
  }

  palabra = ""; //Me aseguro de no repetir la seleccion
  delay(10);
}

//*************funciones****************
void menu(void) {
  if (!volume.init(card)) {       //Se inicia a corde a los formatos permitidos
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);              //accedo a la info
  root.ls(LS_R | LS_DATE | LS_SIZE);  //la imprimo en el orden de datos que quiero
  delay(1000);                        //espero a que aparezcan
  root.close();                       //cierro la configuracion de obtencion de datos


  if (!SD.begin(chipSelect)) {        //Inicio la comunicacion a la SD pero en config. de lectura/escritura
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

}
