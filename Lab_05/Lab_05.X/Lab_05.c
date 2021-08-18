/*
 * File:   Lab_05.c
 * Author: Fredy Godoy - 19260
 *
 * Created on August 13, 2021, 5:59 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h> //variable = strchr( cadena, caracter de busqueda );
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LIB.h"

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
char Boton_0;
char Boton_1;
unsigned char Contador = 0;
char text[16];
//--------------------- Prototipo funci?n configuraci?n ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Interrupcion del PORTB
    if (RBIF == 1){
        if(RB0 == 0){
            Boton_0 = 1;
        }
        if(RB1 == 0){
            Boton_1 = 1;
        }
        RBIF = 0; // Apagar bandera del puerto B
    }// Fin de interrupci?n del PORTB   
    
    // Interrupcion USART
    if (RCIF == 1){
        // RCREG (Receptor)
        PORTA = RCREG; // Valor del sensor Adafruit
        // TXREG (Transmisor)
        RCIF = 0; 
    } // Fin Interrupci?n USART
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuraci?n del progama
    Config_Oscilador();
    Config_USART();
    Config_PORTB();

//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){
        if (Boton_0 == 1 && RB0 == 1){
            Contador++;
            Boton_0 = 0;
            Boton_1 = 0;
        }else if(Boton_1 == 1 && RB1 == 1){
            Contador--;
            Boton_0 = 0;
            Boton_1 = 0;
        }
        sprintf(text,"%d",Contador);
        Texto_USART(text);
        __delay_ms(200);
         
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0;
    //TRISB  = 0;
    //TRISC  = 0;
    TRISE  = 0;
    TRISD  = 0;
    
    ANSEL  = 0;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}
