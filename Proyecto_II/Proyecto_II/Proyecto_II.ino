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
//***************************************/***************************************
//------------------- Notas musicales / variabes buzzer -------------------------
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

#define NOTE_0 0
#define NOTE_1 333
#define NOTE_2 667
#define NOTE_3 1000
#define NOTE_4 1333
#define NOTE_5 1667
#define NOTE_6 2000
#define NOTE_7 2333
#define NOTE_8 2667
#define NOTE_9 3000
#define NOTE_10 3333
#define NOTE_11 3667
#define NOTE_12 4000
#define NOTE_13 4333
#define NOTE_14 4667
#define NOTE_15 5000

int tempo = 150;
int buzzer = PF_1;

int melody[] = {
  NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, 
  NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_B3, 8, NOTE_D4, 8, 
  NOTE_C4, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_E3, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8,
  NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8,
  NOTE_D4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 8,
  NOTE_B3, 8, NOTE_A3, 8,
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
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

const int chipSelect = PA_3;
 
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
File myFile;
File root;
String Numero;
extern uint8_t Imagen_1 [];
extern uint8_t Mapa_2 [];
extern uint8_t Menu_J1 [];
//extern uint8_t Menu_J2 [];
unsigned char temp_2 [25000] PROGMEM = {};
int Sw1_Flag = 0;
int Sw2_Flag = 0;
int SwJ_Flag = 0;  // Switch Joystick
int N_Imagen=0;
boolean Mostrar;
int vidaa = 100;
int pos_x = 212;
int pos_y = 118;
int x_Contrario = 103;
int y_Contrario = 118;
int Dato1; 
int Dato2; 
int Dato3; 
int Dato4; 
int Dato5; 
int Dato6;
// variables funcion linterna
int last_direccion,last_linterna_x,last_linterna_y; 
// variables funcion cofre_loot
int Cofre_Arma;
int Cofre_Balas;
// variables funcion habilitar_armas/balas
int Interactuar = 0;
int ArmaEnable = 0;
int BalasEnable = 0;
int Balas = 0;
//Variables funcion teletransportar
int Llaves = 2;
//--*-*-*-*-* Variables de prueba -*-*-*-*-*-*-
String Jugador = "";
String Contrincante = "";
int direccion;
int Disparar;
const long intervalo = 170;
unsigned long previousMillis,currentMillis;

int Vida_Dr=100;
int Vida_Presa=100;
int Estado = 1;
int Ready_J1 = 0;
int Ready_J2 = 48;

int ColorAccion;
int DatosRecibidos1;
int DatosRecibidos2;
int DatosRecibidos3;

int UltimaPocicion;
int posicion_y;
int primerCero;
int char1;
int char2;
int x;
int x_seg_ciclo;
// Variable estado 3
char temp1;
String x_contrario_temp = "";
String y_contrario_temp = "";
int cont = 0;

extern uint8_t MatrizFondo[];

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
  Serial2.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  Serial2.print("Start");
  LCD_Init();
  LCD_Clear(0x00);

  // SD
  SPI.setModule(0);
  pinMode(PA_3, OUTPUT); // CS pin modulo SD
  //Serial.println("Seleccione 1:Mario, 2:Guitarra, 3:Einstein");
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  root = SD.open("/");
  //printDirectory(root, 0);
  //-------------------------------------------------------------

  //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
  FillRect(80, 60, 160, 120, 0x0400);

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  String text1 = "Iniciando";
  LCD_Print(text1, 85, 110, 2, 0xffff, 0x0000);

  delay(1000);

  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);  
  LCD_Clear(0x00);
  //Mapa();
//  Cofres_Loot ();
//  LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
//  LCD_Sprite(310, 3, 10, 10, LlavesImagen, 1, 0, 0, 0);
//  //Menu("J1");
  //LCD_Bitmap(0, 30, 320, 180, Menu_J1);
//  Informacion_J2 (Vida_Dr);

  
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~         J1 = niño             J2 = doctor              ~~~
  
  Jugador = "J1";
  
  if(Jugador == "J1"){
    Contrincante = "J2";
    ColorAccion =  0xF900;
  }else{
    Contrincante = "J1";
    ColorAccion =  0xF04c0;
  }
  
  // ~~~                                                        ~~~
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {
/*Estados:
  1: Menú de espera de ambos jugadores
  2: Reproducción intro
  3: Jugando
  4: Gana J1 (niño)
  5: Gana J2 (Dr)*/
  if (Estado == 1){
    musica_menu(); // Estado 1 se encuentra en la función musica()
  }

  else if (Estado == 2){
    Estado_2();
  }
  
  else if (Estado == 3){
        
    Movimiento(Jugador);
    currentMillis = millis();
    if (currentMillis - previousMillis >= intervalo) {
      Datos_Comunicacion();
      previousMillis = currentMillis;
      Linterna(pos_x,pos_y,direccion);
      Mapa();
    }
    Sw_Flags();
    if(digitalRead(PUSH1) == LOW && Sw1_Flag == 0){
      Sw1_Flag = 1;
      // Accion del primer boton
      LCD_Bitmap(0, 0, 320, 240, Mapa_2);
      delay(10);
    }
    if(digitalRead(PUSH2) == LOW && Sw2_Flag == 0){
      Sw2_Flag = 1;
      // Accion del segundo boton
      //LCD_Bitmap(0, 30, 320, 180, Menu_J1 );
      if(Jugador == "J1"){ 
        if(ArmaEnable == 1 && BalasEnable == 1 && Balas>0){
          Disparar = 1;
          musica_bala();
          Balas-=1;
          LCD_Print(String(Balas), 280, 3, 1, 0xffff, 0x0000);
        }
      }
      else{
        Disparar = 1;
      }
      delay(10);
    }
    if(digitalRead(Joystick_Push) == LOW && SwJ_Flag == 0){
      SwJ_Flag = 1;
      // Accion del boton Joystick
      Interactuar = 1;
    }
    
    if (Vida_Dr == 0){
      Serial.println("Muere Dr");
      ArmaEnable = 0;
      BalasEnable = 0;
      Vida_Dr = 100;
      Vida_Presa = 100;
      Balas = 0;
      Llaves = 2;
      Estado = 4; 
      Disparar = 0;
    }else if ((Balas == 0 && Vida_Dr > 20 && ArmaEnable == 1 && BalasEnable == 1)||Vida_Presa < 0){
      Serial.println("Muere niño");
      ArmaEnable = 0;
      BalasEnable = 0;
      Vida_Dr = 100;
      Vida_Presa = 100;
      Balas = 0;
      Llaves = 2;
      Estado = 5;
      Disparar = 0;
    }//else if (Vida_Presa < 0){
//      Serial.println("Muere niño");
//      ArmaEnable = 0;
//      BalasEnable = 0;
//      Vida_Dr = 100;
//      Vida_Presa = 100;
//      Balas = 0;
//      Llaves = 2;
//      Estado = 5; 
//      Disparar = 0;
//    }
  }

  else if (Estado == 4){
    Estado_4();
  }

  else if (Estado == 5){
    Estado_5();
  }
  // Prueba de funciones
  
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
void Sw_Flags(void){
  // Actualización bandera de botones
  if(digitalRead(PUSH1) == HIGH){ // Boton 1
    Sw1_Flag = 0;
  }
  if(digitalRead(PUSH2) == HIGH){ // Boton 2
    Sw2_Flag = 0;
  }
  if(digitalRead(Joystick_Push) == HIGH){ // Boton Joystick
    SwJ_Flag = 0;
  }
}
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
        //Ultima_Posicion = myFile.position();
      }else{
        Contador_General++;  
        if(y == 240){
          y = 0;
        }
      }
      //Serial.println(temp);
      
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
  if (Jugador == "J1"){
    LCD_Print("Vida del Doctor", 0, 0, 1, 0xffff, 0x0000);
  }else {
    LCD_Print("Vida de la Presa", 0, 0, 1, 0xffff, 0x0000);
  }
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

//void Informacion_J1(int vida){
//  //int barra = map(vida,0,100,130,200);
//  LCD_Print("Vida de la Presa", 0, 0, 1, 0xffff, 0x0000);
//  // Rojo 0x19e92e, Verde 0xed1c24
//  H_line(129,3,101,0x5419);
//  H_line(129,9,101,0x5419);
//  V_line(129,3,6,0x5419);
//  V_line(231,3,6,0x5419);
//  H_line(130, 4, 100, 0x00);
//  H_line(130, 5, 100, 0x00);
//  H_line(130, 6, 100, 0x00);
//  H_line(130, 7, 100, 0x00);
//  H_line(130, 8, 100, 0x00);
//  
//  if(vida >= 0){
//    H_line(130, 4, vida, 0xE800);
//    H_line(130, 5, vida, 0xE800);
//    H_line(130, 6, vida, 0xE800);
//    H_line(130, 7, vida, 0xE800);
//    H_line(130, 8, vida, 0xE800); 
//  }
//  //for(int x = 130; x<=barra;x+=5){ LCD_Bitmap(x, 4, 5, 5, cuadro_vida); }
//}

void Menu(String J){
  Serial.println("Menu_1");
  if(J == "J1"){
//    LCD_Bitmap(0, 0, 320, 240, Mapa_2);
    Serial.println("/*/*/*/*/*/*/*/*/");
//    LCD_Bitmap(0, 30, 320, 180, Menu_J1);
  }
}

void Movimiento(String jugador){ // J1, J2
  int copia_x,copia_y;
  int valido;
  copia_x = pos_x;
  copia_y = pos_y;
  int px1 = 58;
  int py1 = 21;
  int px2 = 301;
  int py2 = 98;
  int px3 = 31;
  int py3 = 128;
  int px4 = 248;
  int py4 = 206;
  int valor_random;  

  int cx1 = 1;
  int cy1 = 16;
  int cx2 = 83;
  int cy2 = 16;
  int cx3 = 220;
  int cy3 = 16;
  int cx4 = 303;
  int cy4 = 16;
  int cx5 = 1;
  int cy5 = 123;
  int cx6 = 140;
  int cy6 = 191;
  int cx7 = 163;
  int cy7 = 191;
  int cx8 = 303;
  int cy8 = 123;
  
  // Portales
  if(Interactuar == 1){
     Serial.println("----------------------------------------------------------------");
   // randomSeed(analogRead(Joystick_X*Joystick_Y));
   
    Interactuar = 0;
    if(Jugador == "J1"){
      if(pos_x >= px1 && pos_x <= px1+6 && pos_y >= py1 && pos_y <= py1+6){ // portal 1
        do{
          valor_random=random(1,5);
        }while(valor_random == 1);
        if (Llaves > 0){
          Llaves-=1;
          LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px2 && pos_x <= px2+6 && pos_y >= py2 && pos_y <= py2+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 2);
        if (Llaves > 0){
          Llaves-=1;
          LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px3 && pos_x <= px3+6 && pos_y >= py3 && pos_y <= py3+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 3);
        if (Llaves > 0){
          Llaves-=1;
          LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px4 && pos_x <= px4+6 && pos_y >= py4 && pos_y <= py4+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 4);
        if (Llaves > 0){
          Llaves-=1;
          LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      }
      //------------------------------------------------------------------------------------------------    
      //COFRES
      else if (pos_x >= cx1 && pos_x <= cx1+17 && pos_y >= cy1 && pos_y <= cy1+7){
        if(Cofre_Arma == 1){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 1){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx2 && pos_x <= cx2+17 && pos_y >= cy2 && pos_y <= cy2+7){
        if(Cofre_Arma == 2){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 2){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx3 && pos_x <= cx3+17 && pos_y >= cy3 && pos_y <= cy3+7){
        if(Cofre_Arma == 3){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 3){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx4 && pos_x <= cx4+17 && pos_y >= cy4 && pos_y <= cy4+7){
        if(Cofre_Arma == 4){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 4){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx5 && pos_x <= cx5+17 && pos_y >= cy5 && pos_y <= cy5+7){
        if(Cofre_Arma == 5){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 5){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx6 && pos_x <= cx6+17 && pos_y >= cy6 && pos_y <= cy6+7){
        if(Cofre_Arma == 6){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 6){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx7 && pos_x <= cx7+17 && pos_y >= cy7 && pos_y <= cy7+7){
        if(Cofre_Arma == 7){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 7){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }else if (pos_x >= cx8 && pos_x <= cx8+17 && pos_y >= cy8 && pos_y <= cy8+7){
        if(Cofre_Arma == 8){
          //HABILITAR EL ARMA AL JUAGDOR
          Habilitar_Arma();
        } else if(Cofre_Balas == 8){
          //HABILITAR LAS BALAS AL JUAGDOR
          Habilitar_Balas();
        } 
      }
     // -----------------------------------------------------------------------------------------------   
    }

    if(Jugador == "J2"){
      if(pos_x >= px1 && pos_x <= px1+6 && pos_y >= py1 && pos_y <= py1+6){ // portal 1
        do{
          valor_random=random(1,5);
        }while(valor_random == 1);
        if (Llaves > 0){
          //Llaves-=1;
          //LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px2 && pos_x <= px2+6 && pos_y >= py2 && pos_y <= py2+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 2);
        if (Llaves > 0){
          //Llaves-=1;
          //LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px3 && pos_x <= px3+6 && pos_y >= py3 && pos_y <= py3+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 3);
        if (Llaves > 0){
          //Llaves-=1;
          //LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          } else if(valor_random == 4){
            pos_y = py4;
            pos_x = px4;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      } else if (pos_x >= px4 && pos_x <= px4+6 && pos_y >= py4 && pos_y <= py4+6){
        do{
          valor_random=random(1,5);
        }while(valor_random == 4);
        if (Llaves > 0){
          //Llaves-=1;
          //LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
          if(valor_random == 2){
            pos_y = py2;
            pos_x = px2;
          } else if(valor_random == 3){
            pos_y = py3;
            pos_x = px3;
          } else if(valor_random == 1){
            pos_y = py1;
            pos_x = px1;
          }
          FillRect(1,15,320,210,0x000);
          musica_teletransporte();
          Mapa();
        }
      }
    }
  }
  direccion = 0;
  // Cambio de valor en la posición 
  if(analogRead(Joystick_X) >= 3800 && analogRead(Joystick_Y) <= 2500 && analogRead(Joystick_Y) >= 1500 ){ // Joystick X 
    valido = Restriccion_Movimiento(pos_x+1,pos_y);
    if(valido == 1){
      pos_x += 1; 
      direccion = 1;
    } 
  } else if (analogRead(Joystick_X) <= 200 && analogRead(Joystick_Y) <= 2500 && analogRead(Joystick_Y) >= 1500){  
    valido = Restriccion_Movimiento(pos_x-1,pos_y);
    if(valido == 1){
      pos_x -= 1;
      direccion = 3; 
    } 
    
  } if(analogRead(Joystick_Y) >= 3800 && analogRead(Joystick_X) <= 2500 && analogRead(Joystick_X) >= 1500){ // Joystick Y
    valido = Restriccion_Movimiento(pos_x,pos_y+1);
    if(valido == 1){
      pos_y += 1; 
      direccion = 4;
    } 
  } else if (analogRead(Joystick_Y) <= 200 && analogRead(Joystick_X) <= 2500 && analogRead(Joystick_X) >= 1500){  
    valido = Restriccion_Movimiento(pos_x,pos_y-1);
    if(valido == 1){
      pos_y -= 1; 
      direccion = 2;
    } 
  } 
  
  //Movimiento en el eje x
  if(pos_x > copia_x){ // movimiento derecha
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if(jugador == "J1"){
      LCD_Sprite(pos_x, pos_y, 10, 10, J1, 3, 1, 0, 0);  
    }else{
      LCD_Sprite(pos_x, pos_y, 10, 10, J2, 3, 1, 0, 0);
    }
    V_line(pos_x-1 , pos_y, 10, 0x0000);
    V_line(pos_x+9 , pos_y-1, 10, 0x0000);
  } else if(pos_x <copia_x){ // movimiento izquierda
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if(jugador == "J1"){
      LCD_Sprite(pos_x, pos_y, 10, 10, J1, 3, 1, 1, 0);  
    }else{
      LCD_Sprite(pos_x, pos_y, 10, 10, J2, 3, 1, 1, 0);
    }
    V_line(pos_x-1 , pos_y-1, 10, 0x0000);
    V_line(pos_x+11 , pos_y, 10, 0x0000);
  } 
  // Movimiento en el eje y
  else if (pos_y > copia_y){  // movimiento abajo
    H_line(pos_x-1, pos_y-1, 10, 0x0000);
    H_line(pos_x-1, pos_y+10, 10, 0x0000);
    if(jugador == "J1"){
      LCD_Sprite(copia_x, pos_y, 10, 10, J1, 3, 0, 0, 0);  
    }else{
      LCD_Sprite(copia_x, pos_y, 10, 10, J2, 3, 0, 0, 0);
    }
  }else if (pos_y < copia_y){  // movimiento en arriba
    H_line(pos_x-1, pos_y-1, 10, 0x0000);
    H_line(pos_x-1, pos_y+10, 10, 0x0000);
    if(jugador == "J1"){
      LCD_Sprite(copia_x, pos_y, 10, 10, J1, 3, 2, 0, 0);  
    }else {
      LCD_Sprite(copia_x, pos_y, 10, 10, J2, 3, 2, 0, 0);
    }
  } 
  delay(10);
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
  for(int i = 53-10; i<=78+5;i++){
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
  for(int i = 103-10; i<=213+5;i++){
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
  for(int i = 1-10; i<= 51 +5;i++){
      if(i == x || i-1 == x){
        for(int t = 118-10; t<=118+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 118+5 || y == 118-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 266-10; i<= 316+5;i++){
      if(i == x || i-1 == x){
        for(int t = 118-10; t<=118+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 118+5 || y == 118-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 83-10; i<= 143+5;i++){
      if(i == x || i-1 == x){
        for(int t = 78-10; t<=78+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 78+5 || y == 78-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 173-10; i<= 233 +5;i++){
      if(i == x || i-1 == x){
        for(int t = 78-10; t<=78+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 78+5 || y == 78-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 83-10; i<= 143+5;i++){
      if(i == x || i-1 == x){
        for(int t = 158-10; t<=158+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 158+5 || y == 158-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 173-10; i<= 233 +5;i++){
      if(i == x || i-1 == x){
        for(int t = 158-10; t<=158+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 158+5 || y == 158-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 103-10; i<= 213+5;i++){
      if(i == x || i-1 == x){
        for(int t = 186-10; t<=186+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 186+5 || y == 186-10 ){
          valido = 0; 
        }
      }
    }
  for(int i = 238-10; i<= 263 +5;i++){
      if(i == x || i-1 == x){
        for(int t = 186-10; t<=186+5;t++){
          if(y==t){
            valido = 0; 
          }
        } 
        if(y == 186+5 || y == 186-10 ){
          valido = 0; 
        }
      }
    }

  // Verticales
  for(int i = 16-10; i<=51+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78-11; t<=78+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 36-10; i<=46+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158-11; t<=158+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 16-10; i<=51+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238-11; t<=238+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 56-10; i<=91+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 51-11; t<=51+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 51-11 || x == 51+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 83-10; i<=103+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78-11; t<=78+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 83-10; i<=103+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238-11; t<=238+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 53-10; i<=118+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 266-11; t<=266+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 266-11 || x == 266+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 123-10; i<=183+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 51-11; t<=51+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 51-11 || x == 51+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 133-10; i<=153+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78-11; t<=78+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 133-10; i<=153+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238-11; t<=238+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 108-10; i<=128+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158-11; t<=158+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 146-10; i<=181+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 266-11; t<=266+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 266-11 || x == 266+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 186-10; i<=221+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78-11; t<=78+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 78-11 || x == 78+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 191-10; i<=201+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158-11; t<=158+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 158-11 || x == 158+5){
        valido = 0; 
      }        
    }
  }
  for(int i = 191-10; i<=221+5;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238-11; t<=238+5;t++){
          if(x==t){
            valido = 0; 
          }
        }
      if(x == 238-11 || x == 238+5){
        valido = 0; 
      }        
    }
  }

  return valido;
}

boolean Restriccion_Linterna (int x, int y){
  int valido = 1;
  // bordes  pantalla
  if (x < 0 || x > 320){
    valido = 0;
  }else if(y < 16 || y > 240-15){
    valido = 0;
  }
  // Horizontales 
  for(int i = 53; i<=82;i++){
    if(i == x){
        for(int t = 51; t<=51;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
  }
  for(int i = 103; i<=217;i++){
      if(i == x){
        for(int t = 51; t<=51;t++){
          if(y==t){
            valido = 0; 
          } 
        }
      }
    }
  for(int i = 1; i<= 55;i++){
      if(i == x){
        for(int t = 118; t<=118;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }
  for(int i = 266; i<= 320;i++){
      if(i == x){
        for(int t = 118; t<=118;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }
  for(int i = 83; i<= 147;i++){
      if(i == x){
        for(int t = 78; t<=78;t++){
          if(y==t){
            valido = 0; 
          }
        }
      }
    }
  for(int i = 173; i<= 237; i++){
      if(i == x){
        for(int t = 78; t<=78;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }
  for(int i = 83; i<= 147;i++){
      if(i == x){
        for(int t = 158; t<=158;t++){
          if(y==t){
            valido = 0; 
          }
        }
      }
    }
  for(int i = 173; i<= 237;i++){
      if(i == x){
        for(int t = 158; t<=158;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }
  for(int i = 103; i<= 217;i++){
      if(i == x){
        for(int t = 186; t<=186;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }
  for(int i = 238; i<= 267;i++){
      if(i == x){
        for(int t = 186; t<=186;t++){
          if(y==t){
            valido = 0; 
          }
        } 
      }
    }

  // Verticales
  for(int i = 16; i<=59;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78; t<=78;t++){
        if(x==t){
          valido = 0; 
        }
      }      
    }
  }
  for(int i = 36; i<=50;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158; t<=158;t++){
        if(x==t){
          valido = 0; 
        }
      }      
    }
  }
  for(int i = 16; i<=55;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238; t<=238;t++){
        if(x==t){
          valido = 0; 
        }
      }     
    }
  }
  for(int i = 56; i<=95;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 51; t<=51;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 83; i<=107;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78; t<=78;t++){
        if(x==t){
          valido = 0; 
        }
      }    
    }
  }
  for(int i = 83; i<=107;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238; t<=238;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 53; i<=122;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 266; t<=266;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 123; i<=187;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 51; t<=51;t++){
        if(x==t){
          valido = 0; 
        }
      }           
    }
  }
  for(int i = 133; i<=157;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78; t<=78;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 133; i<=157;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238; t<=238;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 108; i<=132;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158; t<=158;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 146; i<=185;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 266; t<=266;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 186; i<=225;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 78; t<=78;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 191; i<=205;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 158; t<=158;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }
  for(int i = 191; i<=225;i++){ // vertical derecha - 10,  vertical izquierda + 5
    if(i == y){
      for(int t = 238; t<=238;t++){
        if(x==t){
          valido = 0; 
        }
      }
    }
  }

  return valido;
}
void Linterna(int x,int y,int direccion){ // direccion 1=derecha 2 = arriba 3 = izquierda 4 = abajo
  int largo_luz_x,largo_luz_y;
  int color = 0;
  int Localidad;
  int i = x;
  int e = y;

  
  Accion_Disparar();
  
  
  if(last_linterna_x<0)
    last_linterna_x = 0;
  else if(320<last_linterna_x)
    last_linterna_x = 320;
  if(last_linterna_y<16)
    last_linterna_y = 16;
  else if(225<last_linterna_y)
    last_linterna_y = 225;
    
  // Borrar rastro de la ultima posición
  if(last_direccion == 1){
    if(direccion != 1 && direccion != 0){
    FillRect(last_linterna_x,last_linterna_y, 16,11,0x0000);
    }
  } else if(last_direccion == 2){
    if(direccion != 2 && direccion != 0){
    FillRect(last_linterna_x,last_linterna_y, 11,16,0x0000);
    }
  } else if(last_direccion == 3){
    if(direccion != 3 && direccion != 0){
    FillRect(last_linterna_x,last_linterna_y, 16,11,0x0000);
    }
  } else if(last_direccion == 4){
    if(direccion != 4 && direccion != 0){
    FillRect(last_linterna_x,last_linterna_y, 11,16,0x0000);
    }
  }
  
  if(direccion == 1){
   // Movimiento a la derecha 
    x+=10;
    largo_luz_x = 15;
    largo_luz_y = 10; 
    last_linterna_x = x-1;
    last_linterna_y = y-1;
    last_direccion = 1;
    for(int i = y; i<y+largo_luz_y && i<=240;i++){
      if (Restriccion_Linterna(x,i)){
        for(int e=x ; e<x+largo_luz_x;e++){
          if (Restriccion_Linterna(e,i)){
            Localidad = (e*2-2)+((i-1)*640);
            color = Mapa_2[Localidad]*256;
            color += Mapa_2[Localidad+1];
            FillRect(e,i, 1,1,color);
          }
          else{
            e = x+largo_luz_x;
          }
        }
      }
    }  
  } else if(direccion == 2){
   // Movimiento arriba 
    y-=1;
    largo_luz_x = 8;
    largo_luz_y = 15; 
    last_linterna_x = x-1;
    last_linterna_y = y-15;
    last_direccion = 2;
    for(int i = x; i<x+largo_luz_x && i<=320;i++){
      if (Restriccion_Linterna(i,y)){
        for(int e=y ; e>y-largo_luz_y;e--){
          if (Restriccion_Linterna(i,e)){
            Localidad = (i*2-2)+((e-1)*640);
            color = Mapa_2[Localidad]*256;
            color += Mapa_2[Localidad+1];
            FillRect(i,e, 1,1,color);
          }
          else{
            e = y-largo_luz_y;
          }
        }
      }
    } 
  } else if(direccion == 3){
   // Movimiento a la izquierda 
    x-=1;
    largo_luz_x = 15;
    largo_luz_y = 10; 
    last_linterna_x = x-15;
    last_linterna_y = y-1;
    last_direccion = 3; 
    for(int i = y; i<y+largo_luz_y && i<=240;i++){
      if (Restriccion_Linterna(x,i)){
        for(int e=x ; e>x-largo_luz_x;e--){
          if (Restriccion_Linterna(e,i)){
            Localidad = (e*2-2)+((i-1)*640);
            color = Mapa_2[Localidad]*256;
            color += Mapa_2[Localidad+1];
            FillRect(e,i, 1,1,color);
          }
          else{
            e = x-largo_luz_x;
          }
        }
      }
    }    
  } else if(direccion == 4){
   // Movimiento abajo 
    y+=10;
    largo_luz_x = 8;
    largo_luz_y = 15; 
    last_linterna_x = x-1;
    last_linterna_y = y-1;
    last_direccion = 4;
    for(int i = x; i<x+largo_luz_x && i<=320;i++){
      if (Restriccion_Linterna(i,y)){
        for(int e=y ; e<y+largo_luz_y;e++){
          if (Restriccion_Linterna(i,e)){
            Localidad = (i*2-2)+((e-1)*640);
            color = Mapa_2[Localidad]*256;
            color += Mapa_2[Localidad+1];
            FillRect(i,e, 1,1,color);
          }
          else{
            e = y+largo_luz_y;
          }
        }
      }
    }
  }
  Jugador_Contrario(Contrincante,x_Contrario, y_Contrario);
}

void Jugador_Contrario(String J, int x, int y){ // x,y son las posiciones del jugador contrario
  if(last_direccion == 1){ // Derecha
    if(x>pos_x+10 && x<pos_x+10+15 && y>=pos_y-9 && y<=pos_y+10){
      if(J == "J1"){
        LCD_Sprite(x, y, 10, 10, J1, 3, 1, 1, 0);  
      }else {
        LCD_Sprite(x,y, 10, 10, J2, 3, 1, 1, 0);
      }        
    }else{
      FillRect(x,y, 10,10,0x0000);
    }
  }else if(last_direccion == 2){ // Arriba
    if(x>=pos_x-9 && x<=pos_x+9 && y>=pos_y-15-10 && y<=pos_y){
      if(J == "J1"){
        LCD_Sprite(x, y, 10, 10, J1, 3, 1, 1, 0);  
      }else {
        LCD_Sprite(x,y, 10, 10, J2, 3, 1, 1, 0);
      }        
    }else{
      FillRect(x,y, 10,10,0x0000);
    }
  }else if(last_direccion == 3){ // Izquierda
    if(x>=pos_x-15-10 && x<=pos_x && y>=pos_y-9 && y<=pos_y+10){
      if(J == "J1"){
        LCD_Sprite(x, y, 10, 10, J1, 3, 1, 1, 0);  
      }else {
        LCD_Sprite(x,y, 10, 10, J2, 3, 1, 1, 0);
      }        
    }else{
      FillRect(x,y, 10,10,0x0000);
    }
  }else if(last_direccion == 4){ // Abajo
    if(x>=pos_x-9 && x<=pos_x+9 && y>=pos_y+10 && y<=pos_y+10+15){
      if(J == "J1"){
        LCD_Sprite(x, y, 10, 10, J1, 3, 1, 1, 0);  
      }else {
        LCD_Sprite(x,y, 10, 10, J2, 3, 1, 1, 0);
      }        
    }else{
      FillRect(x,y, 10,10,0x0000);
    }
  }
    
}
void Cofres_Loot (){
  
  int random1, random2;

  //OBTENCION DE LOS 2 COFRES RANDOM
  int a = 0;
  //randomSeed(analogRead(Joystick_X*Joystick_Y));
  //random1 = random(1,6);
  do{
    a-=1;
    random1 = random(1,9);
  }while(a>0);
  do{
    a = random1;
    random2 = random(1,9);
  }while(random2 == random1 && a>0);

  switch(random1){
    case 1:
      Cofre_Arma = 1;
      break;
    case 2:
      Cofre_Arma = 2;
      break;
    case 3:
      Cofre_Arma = 3;
      break;
    case 4:
      Cofre_Arma = 4;
      break;
    case 5:
      Cofre_Arma = 5;
      break;
    case 6:
      Cofre_Arma = 6;
      break;
    case 7:
      Cofre_Arma = 7;
      break;
    case 8:
      Cofre_Arma = 8;
      break;
  }  

  switch(random2){
    case 1:
      Cofre_Balas = 1;
      break;
    case 2:
      Cofre_Balas = 2;
      break;
    case 3:
      Cofre_Balas = 3;
      break;
    case 4:
      Cofre_Balas = 4;
      break;
    case 5:
      Cofre_Balas = 5;
      break;
    case 6:
      Cofre_Balas = 6;
      break;
    case 7:
      Cofre_Balas = 7;
      break;
    case 8:
      Cofre_Balas = 8;
      break;
  }

  Serial.println(random1);
  Serial.println(random2);
}

void Habilitar_Arma (){
  if (ArmaEnable == 0){
    musica_cofre();
    ArmaEnable = 1;
    LCD_Sprite(270, 3, 10, 10, ArmaImagen, 1, 0, 0, 0);
    Serial.println("arma habilitada");
  }
}

void Habilitar_Balas (){
  if (BalasEnable == 0){
    musica_cofre();
    BalasEnable = 1;
    Balas = 8;
    LCD_Sprite(290, 3, 10, 10, BalasImagen, 1, 0, 0, 0);
    LCD_Print(String(Balas), 280, 3, 1, 0xffff, 0x0000);
    Serial.println("balas habilitada");
  }
}

void Accion_Disparar(void){
  if(Disparar == 1){ 
    if(last_direccion == 1){// Disparar a la derecha 
    FillRect(pos_x+10,pos_y+3,7,5,ColorAccion);
    if(y_Contrario < pos_y+6 && y_Contrario > pos_y-6 && x_Contrario>pos_x){
      if (Jugador == "J1"){
        Vida_Dr-=20;
        Informacion_J2 (Vida_Dr);
      }
      else{
          Vida_Presa-=34;
          Informacion_J2 (Vida_Presa);
      }
    }
    Disparar = 0;
    Serial.println("Se Disparo a la derecha");
    delay(300);
    FillRect(pos_x+10,pos_y+3,7,5,0x8c00);
    } else if(last_direccion == 2){ // Disparar arriba 
      FillRect(pos_x+2,pos_y-7,4,7,ColorAccion);
      if(x_Contrario < pos_x+6 && x_Contrario > pos_x-6 && y_Contrario <pos_y){
        if (Jugador == "J1"){
          Vida_Dr-=20;
          Informacion_J2 (Vida_Dr);
        }
        else{
            Vida_Presa-=34;
            Informacion_J2 (Vida_Presa);
        }
      }
      Disparar = 0;
      Serial.println("Se Disparo para arriba");
      delay(300);
      FillRect(pos_x+2,pos_y-7,4,7,0x8c00);
    }else if(last_direccion == 3){ // Disparar a la izquierda 
      FillRect(pos_x-7,pos_y+3,7,5,ColorAccion);
      if(y_Contrario < pos_y+6 && y_Contrario > pos_y-6 && x_Contrario <pos_x){
        if (Jugador == "J1"){
          Vida_Dr-=20;
          Informacion_J2 (Vida_Dr);
        }
        else{
            Vida_Presa-=34;
            Informacion_J2 (Vida_Presa);
        }
      }
      Disparar = 0;
      Serial.println("Se Disparo a la izquierda");
      delay(300);
      FillRect(pos_x-7,pos_y+3,7,5,0x8c00);
    }else if(last_direccion == 4){ // Disparar abajo
      FillRect(pos_x+2,pos_y+10,4,7,ColorAccion);
      if(x_Contrario < pos_x+6 && x_Contrario > pos_x-6 && y_Contrario>pos_y){
        if (Jugador == "J1"){
          Vida_Dr-=20;
          Informacion_J2 (Vida_Dr);
        }
        else{
            Vida_Presa-=34;
            Informacion_J2 (Vida_Presa);
        }
      }
      Disparar = 0;
      Serial.println("Se Disparo para abajo");
      delay(300);
      FillRect(pos_x+2,pos_y+10,4,7,0x8c00);
    }
  }
}
String ConversionASCII(String x) {
  if (x == "48"){
    return "0";
  }else if (x == "49"){
    return "1";
  }else if (x == "50"){
    return "2";
  }else if (x == "51"){
    return "3";
  }else if (x == "52"){
    return "4";
  }else if (x == "53"){
    return "5";
  }else if (x == "54"){
    return "6";
  }else if (x == "55"){
    return "7";
  }else if (x == "56"){
    return "8";
  }else if (x == "57"){
    return "9";
  }else if (x == "44"){
    Estado = 4;
    return "";
  }else if (x == "46"){
    Estado = 5;
    return "";
  }else {
    return "";
  }
}

//int ConversionDecimal (int a, int b){
//  return (ConversionASCII(a)*16+ConversionASCII(b));
//}

void Estado_1(void){
  if(Jugador == "J1"){LCD_Print("JUGADOR 1", 90, 20, 2, 0xffff, 0x0000);}
    else{LCD_Print("JUGADOR 2", 70, 20, 2, 0xffff, 0x0000);}

    LCD_Print("Presiona el", 75, 70, 2, 0xffff, 0x0000);
    LCD_Print("JOY STICK", 90, 100, 2, 0xffff, 0x0000);
    LCD_Print("para comenzar", 60, 130, 2, 0xffff, 0x0000);
    
    Sw_Flags();
    if(digitalRead(Joystick_Push) == LOW && SwJ_Flag == 0){
      SwJ_Flag = 1;
      // Accion del boton Joystick
      Ready_J1  = 1;
    }
    
    if (Serial2.available()){
      Ready_J2 = Serial2.read();
      //Serial.println("esta leyendo");
    }
    if (Ready_J1){
      Serial2.print(1);  
      //Serial.println("esta mandando la bandera");
    }
    Serial.print("Ready_J1: ");
    Serial.print(Ready_J1);
    Serial.print(" Ready_J2: ");
    Serial.println(Ready_J2);
    
    if (Ready_J1 != 0 && Ready_J2 == '1'){
       Ready_J1 = 0;
       LCD_Clear(0x00);
       Estado = 2;  
    }

    if(Jugador == "J1"){ 
      pos_x = 212;
      pos_y = 118;

      x_Contrario = 103;
      y_Contrario = 118;
      
    }else {
      pos_x = 103;
      pos_y = 118;

      x_Contrario = 212;
      y_Contrario = 118;
      
    }
}

void Estado_2(void){
  Ready_J2 = 48;
    Serial.println("CORRE CINEMATICA");
    // COLOCAR LA FUNCION PARA LA CINEMATICA
    Cargar_Imagen("INTRO.TXT");
    
    Estado = 3;
    LCD_Clear(0x00);
    if(Jugador == "J1"){ 
      Informacion_J2 (Vida_Dr);
      Cofres_Loot ();
      LCD_Print(String(Llaves), 300, 3, 1, 0xffff, 0x0000);
      LCD_Sprite(310, 3, 10, 10, LlavesImagen, 1, 0, 0, 0);
    } else if(Jugador == "J2") {
      Informacion_J2 (Vida_Presa);
    }
}

void Estado_3(){
  
}

void Estado_4(){
  Ready_J2 = 48;
  Serial2.print(',');
  LCD_Clear(0x00);
  //Función para imprimir imagen J1 WIN

  Cargar_Imagen("J1_WIN.TXT");

  LCD_Print("Gana Chico Abducido", 10, 200, 2, 0xffff, NULL);
  LCD_Print("PRESIONA EL JOYSTICK PARA CONTINUAR", 25, 220, 1, 0xffff, NULL);
  
  Serial.println("Gana J1");
  int a = 1;
  while (a){
    if(digitalRead(Joystick_Push) == LOW && SwJ_Flag == 0){
      SwJ_Flag = 1;
      // Accion del boton Joystick
      a = 0;
    }
  }
  LCD_Clear(0x00);
  Estado = 1;
  tempo = 150;
  int melody[] = {
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, 
    NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_B3, 8, NOTE_D4, 8, 
    NOTE_C4, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_E3, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8,
    NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8,
    NOTE_D4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 8,
    NOTE_B3, 8, NOTE_A3, 8,
  };
  notes = sizeof(melody) / sizeof(melody[0]) / 2;
  
  // this calculates the duration of a whole note in ms
  wholenote = (60000 * 4) / tempo;
  
  divider = 0, noteDuration = 0;
}

void Estado_5(){
  Ready_J2 = 48;
  Serial2.print('.');
  LCD_Clear(0x00);
  //Función para imprimir imagen J2 WIN
  Cargar_Imagen("J2_WIN.TXT");

  LCD_Print("Gana Dr Zombie ", 50, 200, 2, 0xffff, NULL);
  LCD_Print("PRESIONA EL JOYSTICK PARA CONTINUAR", 25, 220, 1, 0xffff, NULL);
  
  Serial.println("Gana J2");
  int a = 1;
  while (a){
    if(digitalRead(Joystick_Push) == LOW && SwJ_Flag == 0){
      SwJ_Flag = 1;
      // Accion del boton Joystick
      a = 0;
    }
  }
  LCD_Clear(0x00);
  Estado = 1;
  tempo = 150;
  int melody[] = {
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, 
    NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_B3, 8, NOTE_D4, 8, 
    NOTE_C4, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_E3, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8,
    NOTE_E4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_B3, 8, NOTE_D4, 8, NOTE_D4, 8,
    NOTE_D4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 8,
    NOTE_B3, 8, NOTE_A3, 8,
  };
  notes = sizeof(melody) / sizeof(melody[0]) / 2;
  
  // this calculates the duration of a whole note in ms
  wholenote = (60000 * 4) / tempo;
  
  divider = 0, noteDuration = 0;
    
}

void musica_menu(){ // Estado 1 se encuantra adentro de musica()
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
      Estado_1();
      if(Estado != 1){
        break;
      }
      // calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
  
      // we only play the note for 90% of the duration, leaving 10% as a pause
      tone(buzzer, melody[thisNote], noteDuration * 0.9);
  
      // Wait for the specief duration before playing the next note.
      delay(noteDuration);
  
      // stop the waveform generation before the next note.
      noTone(buzzer);
    }
}

void Datos_Comunicacion(void){
  //ENVIO DE POSICION
    Dato1 = pos_x / 100;
    Dato2 = (pos_x - (Dato1 * 100))/10;
    Dato3 = pos_x - (Dato1 * 100) - (Dato2 * 10);

    Dato4 = pos_y / 100;
    Dato5 = (pos_y - (Dato4 * 100))/10;
    Dato6 = pos_y - (Dato4 * 100) - (Dato5 * 10);

    Serial2.print (Dato1);
    Serial2.print (Dato2);
    Serial2.print (Dato3);
    Serial2.print (Dato4);
    Serial2.print (Dato5);
    Serial2.print (Dato6);
   

    // Lectura de datos (enviados por el otro jugador)
    if (Serial2.available()){
      int temp1;
      x_contrario_temp = "";
      y_contrario_temp = "";
          
      Serial.println(","+String(Dato1)+String(Dato2)+String(Dato3)+String(Dato4)+String(Dato5)+String(Dato6));
      delay(10);
      x_contrario_temp = ConversionASCII(String(Serial2.read()));
      delay(10);
      x_contrario_temp += ConversionASCII(String(Serial2.read()));
      delay(10);
      x_contrario_temp += ConversionASCII(String(Serial2.read()));
      delay(10);
      y_contrario_temp = ConversionASCII(String(Serial2.read()));
      delay(10);
      y_contrario_temp += ConversionASCII(String(Serial2.read()));
      delay(10);
      y_contrario_temp += ConversionASCII(String(Serial2.read()));
      Serial.print("pos_x: " + x_contrario_temp + " pos_y: " + y_contrario_temp);
      Serial.print("    ");
      Serial.println(cont);
      if(cont >= 20){
        Serial2.end();
        Serial2.begin(9600);
        cont = 0;
      } 
    }
    
    x_Contrario = x_contrario_temp.toInt();
    y_Contrario = y_contrario_temp.toInt();
    cont++;
    Jugador_Contrario(Contrincante , x_Contrario, y_Contrario);
}

void musica_cofre(){
  tempo = 150;
  buzzer = PF_1;
  
  int melody[] = {
    NOTE_E6, 16, NOTE_B5, 16,
  };
  notes = sizeof(melody) / sizeof(melody[0]) / 2;
  wholenote = (60000 * 4) / tempo;
  divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void musica_teletransporte(void){
  tempo = 1300;
  buzzer = PF_1;
  int melody[] = {
  NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_1, 8, NOTE_3, 8, NOTE_5, 8, 

    NOTE_9, 8, NOTE_7, 8, NOTE_10, 8, NOTE_9, 8, NOTE_8, 8, NOTE_6, 8, NOTE_7, 8, NOTE_6, 8, NOTE_7, 8, NOTE_6, 8, NOTE_7, 8, NOTE_13, 8, NOTE_10, 8, NOTE_8, 8, NOTE_7, 8, NOTE_8, 8, NOTE_6, 8, NOTE_4, 8, NOTE_9, 8, NOTE_10, 8, 

    NOTE_12, 8, NOTE_9, 8, NOTE_8, 8, NOTE_6, 8, NOTE_5, 8, NOTE_6, 8, NOTE_9, 8, NOTE_8, 8, NOTE_9, 8, NOTE_10, 8, NOTE_9, 8, NOTE_8, 8, NOTE_6, 8, NOTE_5, 8, NOTE_4, 8, NOTE_5, 8, NOTE_7, 8, NOTE_7, 8, NOTE_8, 8, NOTE_9, 8, 

    NOTE_8, 8, NOTE_6, 8, NOTE_3, 8, NOTE_2, 8, NOTE_2, 8, NOTE_3, 8, NOTE_3, 8, NOTE_4, 8, NOTE_4, 8, NOTE_4, 8, NOTE_3, 8, NOTE_3, 8, NOTE_2, 8, NOTE_3, 8, NOTE_2, 8, NOTE_3, 8, NOTE_3, 8, NOTE_4, 8, NOTE_4, 8, NOTE_3, 8, 

    NOTE_3, 8, NOTE_2, 8, NOTE_2, 8, NOTE_1, 8, NOTE_2, 8, NOTE_2, 8, NOTE_2, 8, NOTE_2, 8, NOTE_3, 8, NOTE_2, 8, NOTE_2, 8, NOTE_2, 8, NOTE_2, 8, NOTE_4, 8, NOTE_4, 8, NOTE_5, 8, NOTE_5, 8, NOTE_4, 8, NOTE_4, 8, NOTE_3, 8, 

    NOTE_1, 8, NOTE_0, 8, NOTE_1, 8, NOTE_0, 8, NOTE_0, 8, NOTE_1, 8, NOTE_0, 8, NOTE_1, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8, NOTE_0, 8,
  };
  notes = sizeof(melody) / sizeof(melody[0]) / 2;
  wholenote = (60000 * 4) / tempo;
  divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void musica_bala(){
  tempo = 900;//1300;
  buzzer = PF_1;

  int melody[] = {
    NOTE_1, 8, NOTE_2, 8, NOTE_10, 8, NOTE_8, 8, NOTE_9, 8,
    //NOTE_9, 8, NOTE_11, 8, NOTE_12, 8, NOTE_13, 8, NOTE_14, 8,
  };
  
  notes = sizeof(melody) / sizeof(melody[0]) / 2;
  wholenote = (60000 * 4) / tempo;
  divider = 0, noteDuration = 0;
   for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
