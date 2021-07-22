/*
 * File:   Lab_01.c
 * Author: fredy
 *
 * Created on July 20, 2021, 2:27 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "Libreria.h"
#include "Librerias_II.h"

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (RCIO 
                                     // oscillator: I/O function on RA6/OSC2/ 
                                     // CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
                                // can be enabled by SWDTEN bit of the WDTCON 
                                // register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is digital input, MCLR 
                                // internally tied to VDD)
#pragma config CP = OFF          // Code Protection bit (Program memory code 
                                // protection is enabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code 
                                // protection is enabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/
                                // External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
                                // has digital I/O, HV on MCLR must be used 
                                // for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                // Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                // (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//------------------------------------------------------------------------------
//********************* Declaraciones de variables *****************************
struct{
    char bit0;
    char bit1;
    char bit2;
}Comparador_PORTB;
unsigned char Valor_TMR0 = 100;
unsigned char Valor_ADC;
unsigned char Nibbler_H;
unsigned char Nibbler_L;
unsigned char Boton_0 = 0;
unsigned char Boton_1 = 0;

//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Interrupcion del timer0
    if (T0IF == 1){
        // Interrupcion cada 20ms: tmr0 100, prescaler 256, 8MHz de oscilador
        T0IF = 0;
        TMR0 = Valor_TMR0;
        if (RE0 == 1){
            RE0 = 0;
            PORTC = Tabla_Display(Nibbler_L);
            //PORTC = Tabla_Display(6);
            RE1 = 1;
            
        }else{
            RE1 = 0;
            PORTC = Tabla_Display(Nibbler_H);
            RE0 = 1;
            
        }
    }
    // Interrupcion del PORTB
    if (RBIF == 1){
        if (RB0 == 0){
            PORTD++;
        }else if(RB1 == 0){
            PORTD--;
        }
        RBIF = 0; // Apagar bandera del puerto B
    }// Fin de interrupción del PORTB
    
    // Interrupcion del ADC module
    if (ADIF == 1){
        ADIF = 0;
        Valor_ADC = ADC_Valor(0);
        Nibbler_H = ADC_Valor(0)>>4;
        Nibbler_H = Nibbler_H & 0x0f; 
        Nibbler_L = ADC_Valor(0);
        Nibbler_L = Nibbler_L & 0x0f;
        __delay_us(50);
        ADCON0bits.GO = 1; 
    }
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************    
    // Oscilador
    IRCF0 = 1;       // Configuración del reloj interno 
    IRCF1 = 1;
    IRCF2 = 1;       // 8 Mhz   
    INTCON = 0b11101000;
    
    // Configurar Timer0
    PS0 = 1;
    PS1 = 1;
    PS2 = 1;         //Prescaler de 256
    T0CS = 0;
    PSA = 0;
    INTCON = 0b10101000;
    TMR0 = Valor_TMR0;
    
    // Configuración del modulo ADC
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
    
    // Configuración del puerto B
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0=1;
    WPUBbits.WPUB1=1;
   
    // Activación Interrup on change
    IOCB0 = 1;
    IOCB1 = 1;
    
    
    // Configurar puertos
    ANSEL  = 0b1;
    ANSELH = 0;
    TRISA  = 0xff;       // Definir el puerto A como salida
    TRISB  = 0b00111111; // Definir como salidas los pines 6 y 7 del puerto B
    TRISC  = 0;          // Definir el puerto C como salida
    TRISD  = 0;          // Definir el puerto D como salida
    TRISE  = 0;          // Definir el puerto E como salida
    
    //Limpieza de puertos
    PORTA = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

//------------------------------------------------------------------------------
//*************************** loop principal ***********************************
    while(1){
       
        if (Valor_ADC > PORTD){
            RE2 = 1;
        }else{
            RE2 = 0;
        }
    } // fin loop principal while 
} // fin main
