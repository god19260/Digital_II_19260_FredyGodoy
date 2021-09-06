# Jorge Lanza 19175
# Fernando Caceros 19148
# Fredy Godoy 19260
# Proyecto I- Digital II
from Adafruit_IO import Client, Feed
from requests import NullHandler
import serial
import time
 
ADAFRUIT_IO_USERNAME = "Fredy_Godoy"
ADAFRUIT_IO_KEY = "aio_Kaac27EXBiKG7NBfLc3ndriFcaeS"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
# Claves Feeds
# Contador_1 Feed - Contador Pic
#Contador_1 = aio.feeds('lab-04-digital-ii.contador-1')  # Enlazar feed (clave)

serial_com4 = serial.Serial('COM4',baudrate = 9600, timeout = 1) # port = 'COM4' S

V_Cont_1=0
while True:
    serial_com4.flushInput()
    time.sleep(0.1)
    Var = serial_com4.readline()
    if Var:
        Var = Var.split(sep = '=')
        V_Cont_1 = int(Var[0])
        print(f'Valor Serial(Contador 1): {V_Cont_1}')                
    else:
        print("No hay conexion")

    # Solicitar datos y enviarlos a adafruit
    try:
        time.sleep(0.1)
        # lab
        #V_Contador_2 = int(input("Ingrese Valor Contador 2: "))
        #V_Contador_3 = int(input("Ingrese Valor Contador 3: "))
        # Postlab
        #V_Contador_2 = 30
        #V_Contador_3 = 50
        #if (V_Contador_2 <= 256) & (V_Contador_2 <= 256):
        #    V_Contador_2 = 0
        #    V_Contador_2 = V_Contador_3  
        NullHandler
    except:
        exit()

    
    # Probar lectura y escritura a Adafruit
    try:
        # Esctritura
        # aio.send_data(Contador_1.key,V_Cont_1)      # Enviar datos a la variable 1 - Del PIC a Adafruit
        
        # Lectura
        #Valor_Cont_2 = aio.receive(Contador_2.key)  # Leer datos de la variable
        NullHandler
    except:
        NullHandler

    #Puerto_Pic = int(Valor_Cont_2.value)
    serial_com4.write([]) # Enviar valor del contador 2 a puerto del PIC
    time.sleep(6)