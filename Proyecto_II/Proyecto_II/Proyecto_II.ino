//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
   Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
   Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
   Con ayuda de: José Guerra
   Modificaciones y adaptación: Diego Morales
   IE3027: Electrónica Digital 2 - 2021
*/
//***************************************************************************************************************************************
#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "lcd_registers.h"
#include "font.h"

//***************************************************************************************************************************************
// Variables y Constantes
//***************************************************************************************************************************************
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1

#define Joystick_X PE_3
#define Joystick_Y PE_2
#define Joystick_Push PA_7
 
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
File myFile;
File root;
String Numero;
extern uint8_t Imagen_1 [];
extern uint8_t Mapa_2 [];
extern uint8_t Menu_J1[];
extern uint8_t Menu_J2[];
unsigned char temp_2 [25000] PROGMEM = {};
int Sw1_Flag = 0;
int Sw2_Flag = 0;
int N_Imagen=0;
boolean Mostrar;
int vidaa = 100;

//--*-*-*-*-* Variables de prueba -*-*-*-*-*-*-
int mario_pos = 1;
int mario_index = 1;
int pos_x = 20;
int pos_y = 20;
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);


//extern uint8_t fondo[];
//extern uint8_t Imagen_1[];
//extern uint8_t Imagen_2[];
//extern uint8_t Imagen_3[];
//***************************************************************************************************************************************
// Initialization
//***************************************************************************************************************************************
void setup() {
  //pinMode(Joystick_X, INPUT);
  //pinMode(Joystick_Y, INPUT);
  pinMode(Joystick_Push, INPUT_PULLUP);
  pinMode(PUSH1,INPUT_PULLUP);
  pinMode(PUSH2,INPUT_PULLUP);
  
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);

  // SD
  SPI.setModule(0);
  pinMode(PA_3, OUTPUT); // CS pin modulo SD
  Serial.println("Seleccione 1:Mario, 2:Guitarra, 3:Einstein");
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  root = SD.open("/");
  printDirectory(root, 0);
  //-------------------------------------------------------------

  //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
  FillRect(80, 60, 160, 120, 0x0400);

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  String text1 = "Iniciando";
  LCD_Print(text1, 85, 110, 2, 0xffff, 0x0000);

  delay(1000);

  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);  
  LCD_Clear(0x00);

  Mapa();
    
}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {
  if(digitalRead(PUSH1) == LOW && Sw1_Flag == 0){
    // Accion del primer boton
    LCD_Bitmap(0, 0, 320, 240, Mapa_2);
    delay(10);
  }
  if(digitalRead(PUSH2) == LOW && Sw2_Flag == 0){
    // Accion del segundo boton
    //LCD_Bitmap(0, 30, 320, 180, Menu_J2);
    LCD_Clear(0x0000);
    Mapa();
    delay(10);
  }

  // Prueba de funciones
  Movimiento("J2");

}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      k = k - 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  //Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    //Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height,  unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  }
  else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }
  }
  digitalWrite(LCD_CS, HIGH);
}

//-------------------------------- Funciones SD ----------------------------------------------
// ---------------- Funciones ----------------
void printDirectory(File dir, int numTabs) {
  Serial.println("");
  Serial.println("");
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      //printDirectory(entry, numTabs+1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
  Serial.println("");
  Serial.println("");
}

void Lectura_SD(String nombre) {
  char temp;
  int y = 0;
  char Nombre_Archivo[nombre.length() + 1];
  nombre.toCharArray(Nombre_Archivo, sizeof(Nombre_Archivo));
  // re-open the file for reading:
  myFile = SD.open(Nombre_Archivo);
  if (myFile) {
    Serial.println("");
    Serial.println("");
    Serial.println("Imagen: " + nombre);
    Serial.println("");

    // read from the file until there's nothing else in it:
    for (int x = 0; myFile.available(); x = x + 7) {
      //while (myFile.available()) {
      temp = char(myFile.read());
      Serial.print(temp);
      if (String(temp) == "\n") {
        y = y + 12;
        x = -1;
      }
      LCD_Print(String(temp), x, y, 1, 0xffff, 0x0000);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("No se encontró " + nombre);
  }
}


// ------------------------ Funciones de Programa ------------------------
void Cargar_Imagen(String nombre) {
  char Nombre_Archivo[nombre.length() + 1];
  unsigned char temp; 
  int Resultado,V_MS,V_LS;
  nombre.toCharArray(Nombre_Archivo, sizeof(Nombre_Archivo));
  // re-open the file for reading:
  myFile = SD.open(Nombre_Archivo);
  if (myFile) {
    Serial.println("Imagen: " + nombre);
    // read from the file until there's nothing else in it:
    //for (unsigned int x = 0; myFile.available(); x++) {
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned long Contador_General = 0;
    unsigned int Ultima_Posicion = 0;
    unsigned int Fila = 0;
    unsigned int Size = myFile.size();
    
    if(nombre == "casa.txt"){
      y = 22;
    }else if(nombre =="paisaje.txt"){
      y = 39;
    }
    //myFile.available()
    myFile.seek(2); // Empezar a leer desde la posición 2 del archivo seleccionado
    while (myFile.available()) {
      temp = myFile.read(); // se guarda el caracter leido en la variable local temp
      if(temp == 32){ 
        // Si el caracter es espacio se incrementa x, la cual sirve para cambiar de 
        // espacio en el array que se forma, y para mover dos posiciones para la siguiente lectura. 
        x++;   
        myFile.seek(myFile.position()+2);
      }else if(temp != 44){
        // Si el caracter es diferente a "," se convierte el valor de dos lecturas a decimal y se   
        // agrega a la posición actual del array 
        V_MS = HexToInt(temp);
        temp = myFile.read();
        V_LS = HexToInt(temp);
        Resultado = V_MS*16+V_LS;
        temp_2[x] = Resultado;
      }
      int espacio = 1;
      if(Contador_General >= 1920*espacio){
        LCD_Bitmap(0, y, 320, espacio, temp_2);
        y += espacio;
        x = 0;
        Contador_General = 0;
        if(y == Fila+240){
          Ultima_Posicion = myFile.position();  
        }
        
        //Ultima_Posicion = myFile.position();
      }else{
        Contador_General++;  
        if(y == 240){
          Fila++;
          y = 0;
          myFile.seek(Ultima_Posicion+2); // Empezar a leer desde la segunda linea de pixel
        }
      }
      
    }
    // Mostrar la imagen
    Serial.println("Imagen cargada");
    Serial.println("");
    Serial.println("");
    //LCD_Bitmap(0, 0, 320, 32, temp_2);
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("No se encontró " + nombre);
  }
}


int HexToInt(char hexa){ 
  int entero;
  if(hexa == '0'){entero=0;}
  else if(hexa == '1'){entero=1;}
  else if(hexa == '2'){entero=2;}
  else if(hexa == '3'){entero=3;}
  else if(hexa == '4'){entero=4;}
  else if(hexa == '5'){entero=5;}
  else if(hexa == '6'){entero=6;}
  else if(hexa == '7'){entero=7;}
  else if(hexa == '8'){entero=8;}
  else if(hexa == '9'){entero=9;}
  else if(hexa == 'a'){entero=10;}
  else if(hexa == 'b'){entero=11;}
  else if(hexa == 'c'){entero=12;}
  else if(hexa == 'd'){entero=13;}
  else if(hexa == 'e'){entero=14;}
  else if(hexa == 'f'){entero=15;}
  
  return entero;
}

void Sw_Flags(void){
  // Actualización bandera de botones
  if(digitalRead(PUSH1) == HIGH){ // Boton 1
    Sw1_Flag = 0;
  }
  if(digitalRead(PUSH2) == HIGH){ // Boton 2
    Sw2_Flag = 0;
  }
}

void Mapa(void){ //LCD_Bitmap(x, y, 5, 5, ladrillo_gris);
  // bloques verticales
  for(int y = 16; y<=51;y+=5){ LCD_Bitmap(78, y, 5, 5, ladrillo_gris); }
  for(int y = 36; y<=46;y+=5){ LCD_Bitmap(158, y, 5, 5, ladrillo_gris); }
  for(int y = 16; y<=51;y+=5){ LCD_Bitmap(238, y, 5, 5, ladrillo_gris); }
  for(int y = 56; y<=91;y+=5){ LCD_Bitmap(51, y, 5, 5, ladrillo_gris); }
  for(int y = 83; y<=103;y+=5){ LCD_Bitmap(78, y, 5, 5, ladrillo_gris); }
  for(int y = 83; y<=103;y+=5){ LCD_Bitmap(238, y, 5, 5, ladrillo_gris); }
  for(int y = 53; y<=118;y+=5){ LCD_Bitmap(266, y, 5, 5, ladrillo_gris); }
  for(int y = 123; y<=183;y+=5){ LCD_Bitmap(51, y, 5, 5, ladrillo_gris); }
  for(int y = 133; y<=153;y+=5){ LCD_Bitmap(78, y, 5, 5, ladrillo_gris); }
  for(int y = 133; y<=153;y+=5){ LCD_Bitmap(238, y, 5, 5, ladrillo_gris); }
  for(int y = 108; y<=128;y+=5){ LCD_Bitmap(158, y, 5, 5, ladrillo_gris); }
  for(int y = 146; y<=181;y+=5){ LCD_Bitmap(266, y, 5, 5, ladrillo_gris); }
  for(int y = 186; y<=221;y+=5){ LCD_Bitmap(78, y, 5, 5, ladrillo_gris); }
  for(int y = 191; y<=201;y+=5){ LCD_Bitmap(158, y, 5, 5, ladrillo_gris); }
  for(int y = 191; y<=221;y+=5){ LCD_Bitmap(238, y, 5, 5, ladrillo_gris); }

  // bloques horizontales
  for(int x = 53; x<=78;x+=5){ LCD_Bitmap(x, 51, 5, 5, ladrillo_gris); }
  for(int x = 103; x<=213;x+=5){ LCD_Bitmap(x, 51, 5, 5, ladrillo_gris); }
  for(int x = 1; x<=51;x+=5){ LCD_Bitmap(x, 118, 5, 5, ladrillo_gris); }
  for(int x = 266; x<=316;x+=5){ LCD_Bitmap(x, 118, 5, 5, ladrillo_gris); }
  for(int x = 83; x<=143;x+=5){ LCD_Bitmap(x, 78, 5, 5, ladrillo_gris); }
  for(int x = 173; x<=233;x+=5){ LCD_Bitmap(x, 78, 5, 5, ladrillo_gris); }
  for(int x = 83; x<=143;x+=5){ LCD_Bitmap(x, 158, 5, 5, ladrillo_gris); }
  for(int x = 173; x<=233;x+=5){ LCD_Bitmap(x, 158, 5, 5, ladrillo_gris); }
  for(int x = 103; x<=213;x+=5){ LCD_Bitmap(x, 186, 5, 5, ladrillo_gris); }
  for(int x = 238; x<=263;x+=5){ LCD_Bitmap(x, 186, 5, 5, ladrillo_gris); }
}

void Informacion_J2(int vida){
  //int barra = map(vida,0,100,130,200);
  LCD_Print("Vida del Doctor", 0, 0, 1, 0xffff, 0x0000);
  // Rojo 0x19e92e, Verde 0xed1c24
  H_line(129,3,101,0x5419);
  H_line(129,9,101,0x5419);
  V_line(129,3,6,0x5419);
  V_line(231,3,6,0x5419);
  H_line(130, 4, 100, 0x00);
  H_line(130, 5, 100, 0x00);
  H_line(130, 6, 100, 0x00);
  H_line(130, 7, 100, 0x00);
  H_line(130, 8, 100, 0x00);
  
  if(vida >= 0){
    H_line(130, 4, vida, 0xE800);
    H_line(130, 5, vida, 0xE800);
    H_line(130, 6, vida, 0xE800);
    H_line(130, 7, vida, 0xE800);
    H_line(130, 8, vida, 0xE800); 
  }
  //for(int x = 130; x<=barra;x+=5){ LCD_Bitmap(x, 4, 5, 5, cuadro_vida); }
}

void Menu(void){
  
}

void Movimiento(String jugador){ // J1, J2
  int copia_x,copia_y;
  int valido;
  copia_x = pos_x;
  copia_y = pos_y;
  if(analogRead(Joystick_X) >= 3800 && analogRead(Joystick_Y) <= 2500 && analogRead(Joystick_Y) >= 1500 ){ // Joystick X 
    valido = Restriccion_Movimiento(pos_x+1,pos_y);
    if(valido == 1){
      pos_x += 1; 
    } 
  } else if (analogRead(Joystick_X) <= 200 && analogRead(Joystick_Y) <= 2500 && analogRead(Joystick_Y) >= 1500){  
    valido = Restriccion_Movimiento(pos_x-1,pos_y);
    if(valido == 1){
      pos_x -= 1; 
    } 
    
  } if(analogRead(Joystick_Y) >= 3800 && analogRead(Joystick_X) <= 2500 && analogRead(Joystick_X) >= 1500){ // Joystick Y
    valido = Restriccion_Movimiento(pos_x,pos_y+1);
    if(valido == 1){
      pos_y += 1; 
    } 
  } else if (analogRead(Joystick_Y) <= 200 && analogRead(Joystick_X) <= 2500 && analogRead(Joystick_X) >= 1500){  
    valido = Restriccion_Movimiento(pos_x,pos_y-1);
    if(valido == 1){
      pos_y -= 1; 
    } 
  } 
    
  //Movimiento en el eje x
  if(pos_x > copia_x){ // movimiento derecha
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if(jugador == "J1"){
      LCD_Sprite(pos_x, copia_y, 10, 10, J1, 3, 1, 0, 0);  
    }else{
      LCD_Sprite(pos_x, copia_y, 10, 10, J2, 3, 1, 0, 0);
    }
    V_line(pos_x-1 , pos_y, 10, 0x0000);
    V_line(pos_x+11 , pos_y, 10, 0x0000);
  } else if(pos_x <copia_x){ // movimiento izquierda
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if(jugador == "J1"){
      LCD_Sprite(pos_x, copia_y, 10, 10, J1, 3, 1, 1, 0);  
    }else{
      LCD_Sprite(pos_x, copia_y, 10, 10, J2, 3, 1, 1, 0);
    }
    V_line(pos_x-1 , pos_y, 10, 0x0000);
    V_line(pos_x+11 , pos_y, 10, 0x0000);
  } 
  // Movimiento en el eje y
  else if (pos_y > copia_y){  // movimiento arriba
    H_line(pos_x, pos_y-1, 10, 0x0000);
    H_line(pos_x, pos_y+10, 10, 0x0000);
    if(jugador == "J1"){
      LCD_Sprite(copia_x, pos_y, 10, 10, J1, 3, 0, 0, 0);  
    }else{
      LCD_Sprite(copia_x, pos_y, 10, 10, J2, 3, 0, 0, 0);
    }
  }else if (pos_y < copia_y){  // movimiento en abajo
    H_line(pos_x, pos_y-1, 10, 0x0000);
    H_line(pos_x, pos_y+10, 10, 0x0000);
    if(jugador == "J1"){
      LCD_Sprite(copia_x, pos_y, 10, 10, J1, 3, 2, 0, 0);  
    }else {
      LCD_Sprite(copia_x, pos_y, 10, 10, J2, 3, 2, 0, 0);
    }
  } 
  delay(20);
}

int Restriccion_Movimiento(int x,int y){
  int valido = 1;
  // bordes  pantalla
  if (x == 0 || x == 320-10){
    valido = 0;
  }else if(y == 15 || y == 240-15-10){
    valido = 0;
  }

  // Horizontales
  for(int i = 53-10; i<=78+5;i++){  // horizontal abajo +5,  Horizontal arriba -10
    if(i == x || i-1 == x){
      for(int t = 51-10; t<=51+5;t++){
        if(y==t){
          valido = 0; 
        }
      } 
      if(y == 51+5 || y == 51-10 ){
        valido = 0; 
      }        
    }
  }  
  for(int i = 53-10; i<=78+5;i++){
    if(i == x){
      if(y == 51+5 || y == 51-10){
        valido = 0; 
      }
    }
  }
  for(int i = 103-10; i<=213+5;i++){
      if(i == x){
        if(y == 51+5 || y == 51-10){
          valido = 0; 
        }
      }
    }
  for(int i = 1-10; i<= 51 +5;i++){
      if(i == x){
        if(y == 118+5 || y == 118-10){
          valido = 0; 
        }
      }
    }
  for(int i = 266-10; i<= 316+5;i++){
      if(i == x){
        if(y == 118+5 || y == 118-10){
          valido = 0; 
        }
      }
    }
  for(int i = 83-10; i<= 143+5;i++){
      if(i == x){
        if(y == 78 +5 || y == 78-10){
          valido = 0; 
        }
      }
    }
  for(int i = 173-10; i<= 223 +5;i++){
      if(i == x){
        if(y == 78 +5 || y == 78-10){
          valido = 0; 
        }
      }
    }
  for(int i = 83-10; i<= 143+5;i++){
      if(i == x){
        if(y == 158+5 || y == 158-10){
          valido = 0; 
        }
      }
    }
  for(int i = 173-10; i<= 223 +5;i++){
      if(i == x){
        if(y == 158+5 || y == 158-10){
          valido = 0; 
        }
      }
    }
  for(int i = 103-10; i<= 213+5;i++){
      if(i == x){
        if(y == 186 +5 || y == 186-10){
          valido = 0; 
        }
      }
    }
  for(int i = 238-10; i<= 263 +5;i++){
      if(i == x){
        if(y == 186 +5 || y == 186-10){
          valido = 0; 
        }
      }
    }

  // Verticales
  for(int i = 16-10; i<=51+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 36-10; i<=46+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 16-10; i<=51+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 56-10; i<=91+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 51-11 || x == 51+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 83-10; i<=103+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 83-10; i<=103+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 53-10; i<=118+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 266-11 || x == 266+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 123-10; i<=183+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 51-11 || x == 51+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 133-10; i<=153+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 133-10; i<=153+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 108-10; i<=128+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 146-10; i<=181+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 266-11 || x == 266+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 186-10; i<=221+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 191-10; i<=201+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 191-10; i<=221+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }

  return valido;
}
