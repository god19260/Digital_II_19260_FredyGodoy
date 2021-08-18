# Fredy Godoy 19260
# Laboratorio 4 - Digital II

from Adafruit_IO import Client, Feed
import serial
import time
 
ADAFRUIT_IO_USERNAME = "Fredy_Godoy"
ADAFRUIT_IO_KEY = "aio_lfok06FVQKaK7ZnhmscVek0WxcwQ"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
# Claves Feeds
# Contador_1 Feed - Contador Pic
Contador_1 = aio.feeds('lab-04-digital-ii.contador-1')  # Enlazar feed (clave)
# Contador_2 Feed
Contador_2 = aio.feeds('lab-04-digital-ii.contador-2')  # Enlazar feed (clave)


serial_com4 = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S

V_Cont_1=0
while True:
    if serial_com4.readable():
        #Var = serial_com4.readlines(2)
        Var = serial_com4.readline()
        if Var:
            print(f'Valor Serial: {int(Var)}')
            V_Cont_1 = int(Var)
            aio.send_data(Contador_1.key,V_Cont_1)  # Enviar datos a la variable 1
            serial_com4.write(2)
            time.sleep(0.2)
            
        else:
            print('No hay conexión')
            aio.send_data(Contador_1.key,V_Cont_1)  # Enviar datos a la variable 1
    





