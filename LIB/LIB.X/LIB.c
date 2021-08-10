#include <xc.h>
#include "LIB.h"
#include "SPI.h"
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
void Config_TMR0(void){
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
    
}
void Config_PORTB(void){
    // Configuración del puerto B
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0=1;
    WPUBbits.WPUB1=1;
    
    // Activación Interrup on change
    IOCB0 = 1;
    IOCB1 = 1;
    RBIE = 1;
}
void Config_ADC(void){
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1; // FOSC/8
    ADCON1bits.VCFG0 = 0; // Voltajes de referencia en VSS y VDD
    ADCON1bits.VCFG1 = 0; // Voltajes de referencia en VSS y VDD
    ADCON0bits.CHS = 0;
    ADCON1bits.ADFM = 0; // Justificado a la izquierda    
    ADCON0bits.ADON = 1;
    ADIF = 0; // Bandera de interrupción
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
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
    TRISA  = 0b111111;       // Definir el puerto A como salida
    TRISB  = 0;              // Definir el puerto B como salida
    TRISC  = 0b10000000;     // Definir el puerto C como salida
    TRISD  = 0;              // Definir el puerto D como salida
    TRISE  = 0;              // Definir el puerto E como salida
    ANSEL  = 0b1;
    ANSELH = 0;//0b10100;
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
    __delay_us(50);
    char temp;
    ADCON0bits.CHS = canal;   
    temp = ADRESH;
    __delay_us(50);
    ADCON0bits.GO = 1;
    return temp;           
}
void USART_Num(char valor){
    int temp;
    int unidad, decena, centena;
    temp = valor;
    centena = temp/100;
    temp = temp - centena*100;
    decena = temp/10;
    unidad = temp - decena*10; 
    tabla_USART(centena);
    __delay_ms(5);
    tabla_USART(decena);
    __delay_ms(5);
    tabla_USART(unidad);
    __delay_ms(5);
    
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

void Texto_USART(char texto[]){
    char temp = 10;
    char i = 0;
    while(texto[i] != '\0'){
        TXREG = texto[i];
        i++;
        __delay_ms(5);
    }
}
//------------------------------------------------------------------------------
//--------------- Funciones especificas ----------------------------------------
void SPI(volatile char *v1,volatile char *v2){
    RA0 = ~RA0;
    PORTCbits.RC2 = 0;
    __delay_ms(1);
    spiWrite(0b110011);
    
    if(RA0 == 0){
        *v1 = spiRead(); 
        //PORTD = v1;
    }else{
        *v2 = spiRead();
        //PORTB = v2;
    }
    
    __delay_ms(1);
    PORTCbits.RC2 = 0;
    __delay_ms(10);
    
}

void texto_Programa(char v1, char v2){
    char temp = 10;    
    Texto_USART("POT 1: ");
        __delay_ms(5);
        USART_Num(v1);
        __delay_ms(5);
        //TXREG = '\n';
        TXREG = '\r';
        __delay_ms(5);
        Texto_USART("POT 2: ");
        __delay_ms(100);
        USART_Num(v2);
        __delay_ms(5);
        //TXREG = '\n';
        //TXREG = '\n';
        TXREG = '\r';
        TXREG = '\r';
}
void Interfaz(char v1, char v2){
    USART_Num(v1);
    USART_Num(v2);
}
