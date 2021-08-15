import serial

envio = 500

#Defino nombre de comunicación
dev = serial.Serial("COM2",9600)
#tiempo de espera para recibir datos
dev.timeout = 3 

print('Puerto activado con exito')

while True:
    with dev:   #utilice el COM serial
        envio = str(envio)
        dev.write(envio.encode('ascii'))
        print("Se envio el número: {}".format(envio))
        break
print('Puerto apagado con exito')
