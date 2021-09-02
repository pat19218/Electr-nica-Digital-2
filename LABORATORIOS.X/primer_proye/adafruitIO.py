# Código de AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Cristhofer Patzán

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO
from Adafruit_IO import Client, RequestError, Feed
import serial
import time

envio = 184     #dato que viene desde la pág
nulo = 0        #valor nulo
nulo = str(nulo)#char nulo
temporal1 = 0
temporal2 = 0

#ubicación de mi plataforma
ADAFRUIT_IO_KEY = "aio_PBxi203eGstth023a8heM7WJKKNh"  #contraseña
ADAFRUIT_IO_USERNAME = "CRIS_PAT_99"                    #usuario
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#Defino nombre de comunicación
dev = serial.Serial("COM6",9600) 
dev.timeout = 3 #tiempo de espera para recibir datos
time.sleep(1) #para que establezca conexion

print('Puerto activado con exito \n')
#----------------Temperatura y humedad valor---------------------------------

while True:
    with dev:   #utilice el COM serial
        #nombre_feed = cliente.feed('llave_de_mi_feed') ;feed a utilizar
        temp_feed = aio.feeds('temperatura')
        hume_feed = aio.feeds('humedad')
        end_feed = aio.feeds('exit')

        #variable = cliente.receive(mifeed_feed.key) ; Esto para leer valor
        temp_data = aio.receive(temp_feed.key)
        hume_data = aio.receive(hume_feed.key)
        end_data = aio.receive(end_feed.key)
        
        dev.write(b'h') #mando codigo de caracter
        val1 = dev.readline(3).decode('ascii') #recibo 3 bytes del pic
        val1 = int(val1)  #convierto a entero

        
        dev.write(b't') #mando codigo de caracter
        val2 = dev.readline(3).decode('ascii') #recibo 3 bytes del pic
        val2 = int(val2)  #convierto a entero

        if(val2 != temporal2 or val1 != temporal1):
            print(f'temperatura actual: {temp_data.value}')#imprimo valores del feed
            print(f'humedad actual: {hume_data.value} \n')


            print("Escrito en AdaFruit: ")
            print("humedad ")
            print(val1)
            print("Temperatura")
            print(val2)
            
            #cliente.send_data(mifeed.feed.key, valor a enviar) ; Esto para mandar dato IoT
            aio.send_data(hume_feed.key, val1)
            aio.send_data(temp_feed.key, val2)
                        
            print('----------------------------------------------------')
            temporal1 = val1
            temporal2 = val2
            
        valor = int(end_data.value) #chequo si detener la simulación
        if valor == 1:
            break
        time.sleep(1)
print('Puerto apagado con exito')



