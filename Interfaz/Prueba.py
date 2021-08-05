import serial
import time 

pic = serial.Serial('COM4',9600)
time.sleep(2)
dato_recibido = pic.readline()
print(dato_recibido)
pic.close()
