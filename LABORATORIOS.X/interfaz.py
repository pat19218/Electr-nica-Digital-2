from tkinter import *
from time import sleep
import serial
import time
import sys

serEnable = False #habilitar la comunicacion serial
P = ["",""] #array para guardar los valores
puerto = serial.Serial()
#Baudrate igual que el del PIC
puerto.baudrate=9600
puerto.timeout=3#tiempo de espera para recibir un dato cualquiera
#Puerto usado para el serial
puerto.port='COM4'

def serialInit(): #abrir el puerto e indicarlo en consola
    puerto.open()
    print('Puerto activado con exito')
    global serEnable #global y modificar su valor en la funcion
    serEnable = True

def sendDato():
    A = 0
    A = int(Entrada.get())
    if(A>255 or A<0):
        dato3.set("El valor esta fuera de rango")
        Entrada.delete(0,10)
    else:
        puerto.close()
        dato3.set("Valor " + str(Entrada.get()) + " enviado con exito")
        Entrada.delete(0,10)
        puerto.open()
        puerto.write(chr(A).encode())

#creando una ventana iterable con TK
vent=Tk()
vent.title("Comunicacion Serial - PIC16F887") #titulo
vent.geometry("400x300")#dimension
#elementos de la ventana
dato1 = StringVar() #variables que cambian su valor
dato2 = StringVar()
dato3 = StringVar()
tex= Label(vent,textvariable= dato1,bg = "Orange",fg = "White",height = 3, width = 10)
tex2= Label(vent,textvariable= dato2,bg = "Orange",fg = "White",height = 3, width = 10)
texWarn = Label(vent,textvariable = dato3,fg= "Black")
bot1 = Button(vent,text = "Enviar",bg = "Blue",fg = "White",command = sendDato)
bot2 = Button(vent,text = "Conectar",bg = "Green",fg = "White",command = serialInit)
ident1 = Label(vent,text = " POT1:",bg = "Orange",fg = "White",width = 10)
ident2 = Label(vent,text = " POT2:",bg = "Orange",fg = "White",width = 10)
ident3 = Label(vent,text = "Contador:",fg = "Black")
Entrada = Entry(vent)
#posiciones de los elementos
tex.place(x=100,y=200)
ident1.place(x=100,y=198)
tex2.place(x=200,y=200)
ident2.place(x=200,y=198)
ident3.place(x=100,y=90)
Entrada.place(x=100,y=120)
texWarn.place(x=100,y=150)
bot1.place(x=250,y=120)
bot2.place(x=50,y=50)

while (serEnable==False): #crear la ventana pero esperar a que se active para la
    vent.update() #comuicacion

while True:
    with puerto: #para que tome la com serial
        for i in range(2):
            P[i]=puerto.readline().strip() #divide los dos datos enviados
        a = round((float(P[0])*5.00/255),2)#asigna los valores y los convierte en equivalente de
        b = round((float(P[1])*5.00/255),2)#voltaje a 2 decimas
        dato1.set(str(a)+"V") #imprime el valor
        dato2.set(str(b)+"V")
        time.sleep(0.01) #0.01segundo de espera entre lectura
    vent.update()#actualiza los componentes de la pantalla

#cuando se cierra la pantalla, se cierra la com tambien
puerto.close()
print('Puerto apagado')
sys.exit(0)
