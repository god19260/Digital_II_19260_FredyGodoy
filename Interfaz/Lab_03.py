from tkinter import *
import tkinter as tk
import serial
import time

serial_com4 = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S

#--------------------------- Funciones -----------------------------------
#-------------------------------------------------------------------------
def inc():
    #ser.write(root.contador)
    serial_com4.write(b'Hola a todos')      
    #Var = serial_com4.readline()
    Var = "Hola"


    
#--------------------- Configuracion ventana -----------------------------
root =Tk()
root.title('Prueba')

#Var = StringVar()
#Var = serial_com4.readline()
Var = StringVar()

Titulo = Label(root, text="Pruebas Comunicación Serial").pack()

B_Inc = tk.Button(root, text = "Incrementar", command=inc) # Boton de inc
B_Inc.place(x=70, y=43)

#B_Dec = tk.Button(root, text = "Decrementar", command=dec) # Boton de dec
#B_Dec.place(x=180, y=43)

# valores de potenciometros
Valor_Recibido = Label(root, text="Valor a recibido: ")
Valor_Recibido.place(x=70,y=100)
Valor_Recibido = Label(textv=Var) 
Valor_Recibido.place(x=210,y=100)


root.geometry("320x170") 
root.mainloop()