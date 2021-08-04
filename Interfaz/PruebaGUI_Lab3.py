'''------------------------------------------------------------------------------
Autor: Andy Bonilla
Programa: laboratorio 3
Creado: 1 de agosto de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
intefaz gráfica para el laboratorio de comunicacion SPI
-------------------------------------------------------------------------------'''

'''------------------------------------------------------------------------------
-------------------------IMPORTAR LIBRERIAS--------------------------------------
------------------------------------------------------------------------------'''
import tkinter as tk            #se importa libreria de GUI
from tkinter import *           #se importa libreria de GUI
import serial                   #se importa libreria de comunicacion serial
'''------------------------------------------------------------------------------
-----------------------DEFINICION DE OBJETOS------------------------------------
------------------------------------------------------------------------------'''
root = Tk()                     #se le da nombre al objeto principal

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE PUERTO SERIAL-------------------------------
------------------------------------------------------------------------------'''
#DEFINICION DE PUERTO SERIAL
port1=serial.Serial('COM1')                                 #declarar puerto serial y braudeaje
port1.baudrate = 9600                                       #set Baud rate to 9600
port1.bytesize = 8                                          # Number of data bits = 8
port1.parity   ='N'                                         # No parity
port1.stopbits = 1                                          # Number of Stop bits = 1

#variable is stored in the root object
root.counter = 0                #se declara una variables en el objeto

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
#se define funcion para sumar
def plus_clicked():                                          #se define funcion para sumar
    root.counter += 1
    L['text'] = 'Contador: ' + str(root.counter)
    port1.write(0x31)   #se manda 1 en ascii
    print(0x31)

#se define funcion para restar
def minus_clicked():                                          #se define funcion para sumar
    root.counter -= 1
    L['text'] = 'Contador: ' + str(root.counter)
    port1.write(0x32)   #se manda 1 en ascii
    print(0x32)

'''------------------------------------------------------------------------------
----------------------------CUERPO DE INTERFAZ-----------------------------------
------------------------------------------------------------------------------'''
#TITULO
titulo=tk.Label(root,text = "GUI para laboratorio 3, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)

#titulo de la ventana
root.title("GUI Lab3, Electronica Digital 2")                   #le pones titulo al objeto
root.minsize(400,300)                                           #le decis el tamaño a la ventana

#texto para indicar que putas hacen los botones
label = tk.Label(root, text = "Contador para el Portx del PIC") #texto para el cuadro de texto
label.place(x=115,y=50)

#boton de suma
b1 = Button(root, text="Suma", command=plus_clicked)
b1.place(x=150, y=75)

#boton de resta
b2 = Button(root, text="Resta", command=minus_clicked)
b2.place(x=200,y=75)

#POTENCIOMETROS
label_pots=tk.Label(root, text=port1.read)
label_pots.place(x=135, y=150)

#texto indicador
label1 = tk.Label(root, text = "Valor potenciometro 1")        #texto para el cuadro de texto
label1.place(x=70,y=110)                                       #ubicacion del texto para contador
pot1=tk.LabelFrame(root, text="valor")
pot1.place(x=70,y=125)

#POTENCIOMETRO2
#texto indicador
label2 = tk.Label(root, text = "Valor potenciometro 2")        #texto para el cuadro de texto
label2.place(x=210,y=110)                                      #ubicacion del texto para contador


L = Label(root, text="No clicks yet.")                      
L.pack()

'''------------------------------------------------------------------------------
---------------------------------MAIN LOOP---------------------------------------
------------------------------------------------------------------------------'''
root.mainloop()