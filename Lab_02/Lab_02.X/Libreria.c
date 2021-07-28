#include <xc.h>
#include "Libreria.h"
#include "LCD.h"
#define _XTAL_FREQ 8000000
//------------------------------------------------------------------------------
//----------------- Configuraciones --------------------------------------------
void Config_Oscilador(void){
    // Oscilador
    IRCF0 = 1;       // Configuración del reloj interno 
    IRCF1 = 1;
    IRCF2 = 1;       // 8 Mhz   
    INTCON = 0b11101000;    
}
char Config_TMR0(void){
    // 20 ms de interrupcion
    // Configurar Timer0
    char valor_tmr0;
    PS0 = 1;
    PS1 = 1;
    PS2 = 1;         //Prescaler de 256
    T0CS = 0;
    PSA = 0;
    INTCON = 0b10101000;
    valor_tmr0 = 100;
    TMR0 = valor_tmr0;
    return valor_tmr0;
}

void Config_ADC(void){
    
    PIE1bits.ADIE = 1;
    ADIF = 0; // Bandera de interrupción
    ADCON1bits.ADFM = 0; // Justificado a la izquierda    
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG0 = 0; // Voltajes de referencia en VSS y VDD
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1; // FOSC/8
    ADCON0bits.ADON = 1;
    __delay_us(50);
    ADCON0bits.GO = 1;
}
void Config_USART(void){
    // Configuración de Asynchronous TRANSMITTER
    TXEN = 1;
    SYNC = 0;
    SPEN = 1;
   
    // Configuración de Asynchronous RECEIVER
    CREN = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    SPBRG=12;  // baudrate 9600 para 8MHz
}

void Config_Puertos(void){
    // Configurar puertos
    ANSEL  = 0;
    ANSELH = 0;//0b10100;
    TRISA  = 0;       // Definir el puerto A como salida
    TRISB  = 0b11;    // Definir el puerto B como salida
    TRISC  = 0b10000000;       // Definir el puerto C como salida
    TRISD  = 0;       // Definir el puerto D como salida
    TRISE  = 0;       // Definir el puerto E como salida
    
    //Limpieza de puertos
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}
//------------------------------------------------------------------------------
//--------------- Funciones de programa ----------------------------------------
char Valor_ADC(char canal){
    ADCON0bits.CHS = canal;
    __delay_us(50);
    ADCON0bits.GO = 1;
    return ADRESH;           
}

void Transmisor_USART(char valor){
    int temp;
    int unidad, decena, centena;
    temp = valor * 2;
    centena = temp/100;
    temp = temp - centena*100;
    decena = temp/10;
    unidad = temp - decena*10; 
    tabla_USART(centena);
    __delay_ms(5);
    TXREG = '.';
    tabla_USART(decena);
    __delay_ms(5);
    tabla_USART(unidad);
    __delay_ms(5);
    

//    tabla_num(centena);
//    tabla_num(decena);
//    tabla_num(unidad);   
}
void tabla_USART(int numero){
    // forma == 0 LCD
    // forma == 1 USART
    if (numero == 1){
        TXREG = '1'; 
    } else if (numero == 2){
        TXREG = '2';
    } else if (numero == 3){
        TXREG = '3';
    } else if (numero == 4){
        TXREG = '4';
    } else if (numero == 5){
        TXREG = '5';
    } else if (numero == 6){
        TXREG = '6';
    } else if (numero == 7){
        TXREG = '7';
    } else if (numero == 8){
        TXREG = '8';
    } else if (numero == 9){
        TXREG = '9';
    } else if (numero == 0){
        TXREG = '0';
    }
}

void USART(unsigned char canal_10,unsigned char canal_12){
    Texto_USART("POT 1    ");
    Texto_USART("POT 2");
    TXREG = '\r';
    Transmisor_USART(canal_10);
    Texto_USART(" --- ");
    Transmisor_USART(canal_12);
    TXREG = '\r';
}

void Texto_USART(char texto[]){
    char i = 0;
    while(texto[i] != '\0'){
        TXREG = texto[i];
        i++;
        __delay_ms(1);
    }
}
void LCD(unsigned char canal_10,unsigned char canal_12, unsigned char cont){
    Lcd_Set_Cursor(1,2);
    Write_LCD("S1");
    Lcd_Set_Cursor(2,1);
    Print_Num(canal_10);
    
    Lcd_Set_Cursor(1,8);
    Write_LCD("S2");
    Lcd_Set_Cursor(2,7);
    Print_Num(canal_12); 
    
    Lcd_Set_Cursor(1,13);
    Write_LCD("S3");
    Lcd_Set_Cursor(2,13);
    Print_Cont(cont);
//    Lcd_Set_Cursor(1,12);
//    Write_LCD("USART");
//    Lcd_Set_Cursor(2,12);
//    Print_Cont(cont); 
}