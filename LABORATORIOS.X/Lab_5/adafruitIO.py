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
ADAFRUIT_IO_KEY = "_contraseña_aqui_"  #contraseña
ADAFRUIT_IO_USERNAME = "CRIS_PAT_99"                    #usuario
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#Defino nombre de comunicación
dev = serial.Serial("COM2",9600) 
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
        
        valor = int(temp_data.value)
        dev.write(b's') #mando codigo de caracter
        val = dev.readline(3).decode('ascii') #recibo 2 bytes
        val = int(val)  #convierto a entero

        if(valor != temporal1 or val != temporal2):
            print(f'temperatura actual: {temp_data.value}')#imprimo valores del feed
            print(f'humedad actual: {hume_data.value} \n')

            if (valor < 10 and valor > -1):
                dev.write(nulo.encode('ascii')) #envio un cero
                dev.write(nulo.encode('ascii'))
                
            elif (valor <100 and valor > 9):
                dev.write(nulo.encode('ascii')) #envio un cero
            
            envio = str(int(temp_data.value))
            dev.write(envio.encode('ascii'))    #envio un dato
            print("Escrito en pic: {} \n".format(envio))
            print("Escrito en AdaFruit: ")
            print(val)
            
            #cliente.send_data(mifeed.feed.key, valor a enviar) ; Esto para mandar dato y leer
            aio.send_data(hume_feed.key, val)
            print('----------------------------------------------------')
            temporal1 = int(temp_data.value)
            temporal2 = int(hume_data.value)
            
        valor = int(end_data.value) #chequo si detener la simulación
        if valor == 1:
            break
        time.sleep(0.1)
print('Puerto apagado con exito')



