# Fredy Godoy 19260
# Laboratorio 4 - Digital II

from Adafruit_IO import Client, Feed
import serial
import time
 
ADAFRUIT_IO_USERNAME = "Fredy_Godoy"
ADAFRUIT_IO_KEY = "aio_lucj71yKDlH3n2PKryZmmv8BzQO0"

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
serial_com4 = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S

while True:
    Var = serial_com4.readline()
    print(Var)
    # Contador_1 Feed - Contador Pic
    Contador_1 = aio.feeds('lab-04-digital-ii.contador-1')  # Enlazar feed (clave)
    aio.send_data(Contador_1.key,30)  # Enviar datos a la variabel
    dato = aio.receive(Contador_1.key) # Leer datos de la variable
    V_Contador_1 = int(dato.value)
    print(f'Contador Pic: {V_Contador_1}')

    # Contador_2 Feed
    Contador_2 = aio.feeds('lab-04-digital-ii.contador-2')  # Enlazar feed (clave)
    aio.send_data(Contador_1.key,60)  # Enviar datos a la variabel
    dato = aio.receive(Contador_2.key) # Leer datos de la variable
    V_Contador_2 = int(dato.value)
    print(f'Contador 2: {V_Contador_2}')

    # Contador_3 Feed 
    Contador_3 = aio.feeds('lab-04-digital-ii.contador-3')  # Enlazar feed (clave)
    aio.send_data(Contador_3.key,90)  # Enviar datos a la variabel
    dato = aio.receive(Contador_3.key) # Leer datos de la variable
    V_Contador_3 = int(dato.value)
    print(f'Contador 3 {V_Contador_3}')
