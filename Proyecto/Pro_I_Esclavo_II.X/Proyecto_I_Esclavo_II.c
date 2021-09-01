/*
 * Proyecto I ESCLAVO II
 * File:   Proyecto_I.c 
 * Author: Fredy Godoy 19260
 *
 * Created on August 23, 2021, 9:07 AM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LIB.h"
#include "../../LIB/LIB.X/LCD.h"
#include <stdint.h>
#include <stdio.h>

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
char text[16];
unsigned char CNY70 = 0;
char Distancia;
//--------------------- Prototipo función configuración ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
     // Interrupcion del ADC module
    if (ADIF == 1){
        ADIF = 0;
        CNY70 = Valor_ADC(0);  
    } // Fin Interrupcion ADC
    // Interrupcion USART
    if (RCIF == 1){
        // RCREG (Receptor)
        // TXREG (Transmisor)
        if (RCREG == 'c'){
            sprintf(text,"Valor CN70 %d",CNY70);
            Texto_USART(text);
        }            
        RCIF = 0; 
    } // Fin Interrupción USART
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    
    Config_Oscilador(); 
    Config_ADC();
    Config_USART();
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){     
        //PORTB = CNY70;
        Distancia = Ultrasonico();
        sprintf(text,"Distancia: %d",Distancia);
        Texto_USART(text);
        __delay_ms(200);                                  
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0b1;
    TRISB  = 0;
    TRISE  = 0;
    TRISD  = 0xff;
    
    ANSEL  = 0b1;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    PORTB = 0;
   // PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

