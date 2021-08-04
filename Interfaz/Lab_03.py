from tkinter import *
import tkinter as tk
import serial


ser = serial.Serial(baudrate = 9600, timeout = 1) # port = 'COM4' 
def Enviar():
    ser.write(b's') # 

# Configuración de ventana
root =Tk()
root.title('Digital II - Lab 03 - Fredy Godoy 19260')

B_Enviar = tk.Button(root, text = "Enviar", command=Enviar)
B_Enviar.grid(row=0, column=0)

root.geometry("350x350") 
root.mainloop()