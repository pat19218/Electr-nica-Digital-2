String palabra;          //variable en la q ira la palabra 

void setup() {
  Serial.begin(9600);    //habilito la comunicacion
  Serial1.begin(9600);    //habilito la comunicacion
  pinMode(13,OUTPUT);      

}

void loop() {
  while (Serial1.available()){    //Confirmando que la comunicación Serial esté disponible para hacer la lectura del mensaje
    delay(10);                    //para dar estabilidad al programa 
    char c =Serial1.read();       //esta otra mamada almacena la lectura de la comunicacion serial (por asi dexirlo lo que le entra XD jajajajaj)
    palabra += c;                 //sumando el contenido de la variable c en la variable mensaje (osea inserta la palabra a la variable)
  }
  
  if(palabra.length()>0){      //para ver si no esta vacio
    Serial.println(palabra);        //imprime lo recibido
  }
  
  palabra="";
  delay(250);
}
