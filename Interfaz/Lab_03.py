from tkinter import *
import tkinter as tk
import serial
import time

ser = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S
ser.bytesize = 8                                          # Number of data bits = 8
ser.parity   ='N'                                         # No parity
ser.stopbits = 1                                          # Number of Stop bits = 1
#--------------------------- Funciones -----------------------------------
#-------------------------------------------------------------------------
#def Enviar():
#    ser.write(b'1')
def inc():
    #ser.write(root.contador)
    ser.write(b'+')    
def dec():
    #ser.write(root.contador)
    ser.write(b'-')    

    
#--------------------- Configuracion ventana -----------------------------
root =Tk()
root.title('Digital II - Lab 03 - Fredy Godoy 19260')

root.contador = 0 # variable contador
valor = StringVar()
Valor_Pot_1 = StringVar()
#Valor_Pot_2 = StringVar()
serBuffer = ""
def readSerial():
    while True:
        Valor_Pot_1 = ser.read()
        if len(Valor_Pot_1) == 0:
            break
        global serBuffer
         # check if character is a delimeter
        if c == '\r':
            c = '' # don't want returns. chuck it
            
        if c == '\n':
            serBuffer += "\n" # add the newline to the buffer



#time.sleep(0.2)
Valor_Pot_2 = ser.readline()

Titulo = Label(root, text="Lab 03 - SPI/USART").pack()

#Cuadro_Entrada = Entry(root, textvariable = valor)     # Cuadro de entrada
#Cuadro_Entrada.place(x=70,y=45)
#B_Enviar = tk.Button(root, text = "Enviar", command=Enviar) # Boton de enviar
#B_Enviar.place(x=210, y=43)  
B_Inc = tk.Button(root, text = "Incrementar", command=inc) # Boton de inc
B_Inc.place(x=70, y=43)

B_Dec = tk.Button(root, text = "Decrementar", command=dec) # Boton de dec
B_Dec.place(x=180, y=43)

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