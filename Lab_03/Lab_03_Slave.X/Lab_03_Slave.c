/*
 * File:   Lab_03_Slave.c
 * Author: Fredy Godoy 19260
 * PIC Maestro
 * Created on August 3, 2021, 12:41 AM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LIB.h"
#include "../../LIB/LIB.X/SPI.h"

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
char V_ADC_0 = 0;
char V_ADC_1 = 0;
char temp1 = 0;
//--------------------- Prototipo función configuración ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Interrupcion del ADC module
    if (ADIF == 1){
        ADIF = 0;
        V_ADC_0 = Valor_ADC(0);
        V_ADC_1 = Valor_ADC(1);
    }
    if(SSPIF == 1){
        if (temp1 == 0){
            spiWrite(V_ADC_0);
            temp1 = 1;
        } else{
            spiWrite(V_ADC_1);
            temp1 = 0;
        }
        //spiWrite(V_ADC_1);
        // void spiReceiveWait()
        SSPIF = 0;
    } 
    
    // Interrupcion USART
    if (RCIF == 1){
        // RCREG (Receptor)
        // TXREG (Transmisor)

        RCIF = 0; 
    }
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    
    Config_Oscilador();
    Config_ADC();
    Config_USART();
    Init_Slave(); // SPI
    
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){
        

    } // fin loop principal while 
} // fin main

void config(void){   
    TRISA = 0b100011;
    TRISB = 0;
    TRISD = 0;
    
    ANSEL  = 0b11;
    ANSELH = 0;
    
    //Limpieza de puertos
    //PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}