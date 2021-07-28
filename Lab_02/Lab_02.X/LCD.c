#include <xc.h>
#include "LCD.h"
#define _XTAL_FREQ 8000000

void LCD_Init_8bits(void){
    PORT_LCD(0); // Limpiamos el puerto de conexión a LCD
    __delay_ms(20);
    CMD_LCD(0b111000);
    __delay_ms(20);
    CMD_LCD(0b1100);
    __delay_ms(20);
    CMD_LCD(0b1);
    //*-*-*-*-*-*-*-*-*-*
    CMD_LCD(0b111000);
    CMD_LCD(0b1000);
    CMD_LCD(0b1);
    CMD_LCD(0b110);
}

void PORT_LCD(char v){
    D0 = v & 1 ? 1 : 0;
    D1 = v & 2 ? 1 : 0;
    D2 = v & 4 ? 1 : 0;
    D3 = v & 8 ? 1 : 0;
    D4 = v & 16 ? 1 : 0;
    D5 = v & 32 ? 1 : 0;
    D6 = v & 64 ? 1 : 0;
    D7 = v & 128 ? 1 : 0;
}
void CMD_LCD(char v){
    RS = 0; // Comando
    PORT_LCD(v);
    EN = 1; // E = 1
    __delay_us(500);
    EN = 0; // E = 0
}
void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        CMD_LCD(temp);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        CMD_LCD(temp);
    }
}
void Clear_LCD(void){
    CMD_LCD(0);
    CMD_LCD(1);
}
void Char_LCD(char a){
    RS = 1; // RS = 1
    PORT_LCD(a);
    EN = 1;
    __delay_us(500);
    EN = 0;
}
void Write_LCD(char *a) {
    char i;
    for (i = 0; a[i] != '\0'; i++)
        Char_LCD(a[i]);    
}

void Print_Num(char valor){ // imprimir numero LCD
    int temp;
    int unidad, decena, centena;
    temp = valor * 2;
    centena = temp/100;
    temp = temp - centena*100;
    decena = temp/10;
    unidad = temp - decena*10; 
    tabla_num(centena);
    Write_LCD(".");
    tabla_num(decena);
    tabla_num(unidad);  
    
}
void Print_Cont(char valor){ // imprimir numero LCD
    int temp;
    int unidad, decena, centena;
    centena = valor/100;
    temp = valor - centena*100;
    decena = temp/10;
    unidad = temp - decena*10; 
    tabla_num(centena);
    tabla_num(decena);
    tabla_num(unidad);     
}

void tabla_num(int numero){
    
    // forma == 0 LCD
    // forma == 1 USART
    if (numero == 1){
    Write_LCD("1");
    } else if (numero == 2){
        Write_LCD("2");
    } else if (numero == 3){
        Write_LCD("3");
    } else if (numero == 4){
        Write_LCD("4");
    } else if (numero == 5){
        Write_LCD("5");
    } else if (numero == 6){
        Write_LCD("6");
    } else if (numero == 7){
        Write_LCD("7");
    } else if (numero == 8){
        Write_LCD("8");
    } else if (numero == 9){
        Write_LCD("9");
    } else if (numero == 0){
        Write_LCD("0");
    }
}
