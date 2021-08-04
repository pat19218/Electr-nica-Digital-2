from tkinter import *
import time
import serial

n = 1
nota = ""
#creo objeto serial
pic = serial.Serial(port = 'COM4',
                    baudrate = 9600,
                    parity=serial.PARITY_ODD,
                    stopbits=serial.STOPBITS_TWO,
                    bytesize=serial.EIGTHBITS)

entrada = pic.read()
print(entrada)

class Window(Frame):    
    def __init__(self, master = None):
        Frame.__init__(self, master)

        self.master = master
        self.init_window()

    def init_window(self):#inicio de creación para la ventana
        self.master.title("Interfaz") #nombre de la ventana
        self.pack(fill=BOTH, expand=1)

        quitButton = Button(self, text="Quit", command=self.client_exit) #creo el boton
        quitButton.place (x=0, y=0) #ubicación del boton

        quitButton = Button(self, text="valor pot1") #creo el boton
        quitButton.place (x=80, y=180) #ubicación del boton

        quitButton = Button(self, text="valor pot2") #creo el boton
        quitButton.place (x=250, y=180) #ubicación del boton

      

    def client_exit(self):
        exit()
        
root = Tk()
root.geometry("400x300") #tamaño de la ventana

app = Window(root)
#CREACION DE ETIQUETAS 
lb2=Label(root,text='Envia un valor de 0 a 500',width=38,height=4)
lb2.place(x=50,y=60)

#CREACION DE TEXTO
tex1= Text(root,width=10,height=2)
tex1.place(x=150,y=100)

root.mainloop()


    
    
    
