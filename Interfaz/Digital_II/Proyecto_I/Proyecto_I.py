# Jorge Lanza 19175
# Fernando Caceros 19148
# Fredy Godoy 19260
# Proyecto I- Digital II
from Adafruit_IO import Client, Feed
from requests import NullHandler
import serial
import time
 
ADAFRUIT_IO_USERNAME = "Fredy_Godoy"
ADAFRUIT_IO_KEY = "aio_eMmU82WiKfr1bs6IqsebjSW1LZb1"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
# Claves Feeds
# Contador_1 Feed - Contador Pic
Distancia = aio.feeds('proyecto-i-digital-ii.distancia')  # Enlazar feed (clave)
Color = aio.feeds('proyecto-i-digital-ii.color')          # Enlazar feed (clave)
Hora = aio.feeds('proyecto-i-digital-ii.hora')            # Enlazar feed (clave)
Modo = aio.feeds('proyecto-i-digital-ii.modo')            # Enlazar feed (clave)

serial_com = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S

V_Distancia=0
V_Color = 0
V_Hora = ''
V_Modo = 0
while True:
    serial_com.flushInput()
    time.sleep(0.1)
    Var = serial_com.readline()
    try:
        if Var:
            Var = Var.split()
            V_Distancia = int(Var[0])
            V_Color = int(Var[1])
            V_Hora = str(Var[2])
            print(f'Distancia: {V_Distancia}, Color: {V_Color}, Hora: {V_Hora} ')
        else:
            print("No hay conexion")
    except:
        print("Hubo error")

    # Probar lectura y escritura a Adafruit
    try:
        # Esctritura
        aio.send_data(Distancia.key,V_Distancia)      # Enviar datos a la variable 1 - Del PIC a Adafruit
        aio.send_data(Color.key,V_Color)              # Enviar datos a la variable 1 - Del PIC a Adafruit
        aio.send_data(Hora.key,V_Hora)                # Enviar datos a la variable 1 - Del PIC a Adafruit

        # Lectura
        V_Modo = aio.receive(Modo.key)  # Leer datos de la variable

        # Escritura Serial
        Valor_Pic = str(V_Modo.value)
        Valor_Pic = Valor_Pic.encode('utf-8')
        serial_com.write(Valor_Pic) # Enviar valor del contador 2 a puerto del PIC
        print(f'Modo: {Valor_Pic}')
    except:
        print("Hubo error enviando")


    time.sleep(8)