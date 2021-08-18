from Adafruit_IO import Client, Feed
 
ADAFRUIT_IO_USERNAME = "Fredy_Godoy"
ADAFRUIT_IO_KEY = "aio_lucj71yKDlH3n2PKryZmmv8BzQO0"

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#Analog Feed
analog_feed = aio.feeds('adc')  # Enlazar feed
aio.send_data(analog_feed.key, 100)  # Enviar datos a la variabel
analog_data = aio.receive(analog_feed.key) # Leer datos de la variable
print(f'analog signal: {analog_data.value}')

# Pot_1 Feed
Pot_1 = aio.feeds('pot-1')  # Enlazar feed (clave)
aio.send_data(Pot_1.key,30)  # Enviar datos a la variabel
Valor_Pot_1 = aio.receive(Pot_1.key) # Leer datos de la variable
print(f'Pot_1: {Valor_Pot_1.value}')