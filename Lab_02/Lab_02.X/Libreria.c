#include <xc.h>
#include "Libreria.h"
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
    char unidad, decena, centena;
    centena = valor/100;
    valor = valor - centena*100;
    decena = valor/10;
    unidad = valor - decena*10; 
    tabla_USART(centena);
    __delay_ms(1);
    tabla_USART(decena);
    __delay_ms(1);
    tabla_USART(unidad);
    __delay_ms(1);
    TXREG = '\r';

//    tabla_num(centena);
//    tabla_num(decena);
//    tabla_num(unidad);   
}
void tabla_USART(char numero){
    // forma == 0 LCD
    // forma == 1 USART
    if (numero == 1){
        TXREG = '1'; 
    } else if (numero == 2){
        TXREG = '2';
    } else if (numero == 3){
       
    } else if (numero == 4){
        
    } else if (numero == 5){
        
    } else if (numero == 6){
        
    } else if (numero == 7){
        
    } else if (numero == 8){
        
    } else if (numero == 9){
        
    } else if (numero == 0){
        
    }
}
