from tkinter import *
import tkinter as tk
import serial


ser = serial.Serial(baudrate = 9600, timeout = 1) # port = 'COM4' 
#--------------------------- Funciones -----------------------------------
#-------------------------------------------------------------------------
def Enviar():
    ser.write(root.contador)    

    
#--------------------- Configuracion ventana -----------------------------
root =Tk()
root.title('Digital II - Lab 03 - Fredy Godoy 19260')

root.contador = 0 # variable contador
valor = StringVar()
#Valor_Pot_1 = StringVar()
#Valor_Pot_2 = StringVar()
Valor_Pot_1 = ser.read()
Valor_Pot_2 = ser.read()

Titulo = Label(root, text="Lab 03 - SPI/USART").pack()

Cuadro_Entrada = Entry(root, textvariable = valor)     # Cuadro de entrada
Cuadro_Entrada.place(x=70,y=45)
  
B_Enviar = tk.Button(root, text = "Enviar", command=Enviar) # Boton de enviar
B_Enviar.place(x=210, y=43)

# valores de potenciometros

Pot_1 = Label(root, text="Valor potenciometro 1")
Pot_1.place(x=70,y=100)
V_Pot_1 = Label(textvariable=Valor_Pot_1) 
V_Pot_1.place(x=210,y=100)

Pot_2 = Label(root, text="Valor potenciometro 2")
Pot_2.place(x=70,y=120)
V_Pot_2 = Label(textvariable=Valor_Pot_2) 
V_Pot_2.place(x=210,y=120)

root.geometry("320x170") 
root.mainloop()