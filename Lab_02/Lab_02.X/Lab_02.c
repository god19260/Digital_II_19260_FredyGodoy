/*
 * File:   Lab_02.c
 * Author: Fredy Godoy
 *
 * Created on July 27, 2021, 1:29 AM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "Libreria.h"
#include "LCD.h"

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
unsigned char Valor_TMR0 = 100;
unsigned char Valor_Canal_10;
unsigned char Valor_Canal_12;
unsigned char contador = 0;
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
 // Interrupcion del timer0
    if (T0IF == 1){
        // Interrupcion cada 20ms: tmr0 100, prescaler 256, 8MHz de oscilador
        T0IF = 0;
        TMR0 = Valor_TMR0;
           
    } // Fin de interrupción timer0
    
    // Interrupcion del ADC module
    if (ADIF == 1){
        ADIF = 0;
        Valor_Canal_12 = Valor_ADC(12);
        __delay_ms(10);
        Valor_Canal_10 = Valor_ADC(10);
        //Transmisor_USART(Valor_Canal_12);
    }
    
    // Interrupcion USART
    if (RCIF == 1){
        // RCREG (Receptor)
        // TXREG (Transmisor)
        if (RCREG == '+'){
            contador++;
        }else if (RCREG == '-'){
            contador--;
        }
        RCIF = 0; 
    }
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    Config_Oscilador();
    Valor_TMR0 = Config_TMR0();
    Config_ADC();
    Config_USART();
    Config_Puertos();
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************
    LCD_Init_8bits();
    
    while(1){
        USART(Valor_Canal_10, Valor_Canal_12);
        LCD(Valor_Canal_10,Valor_Canal_12,contador);
        __delay_ms(100);

    } // fin loop principal while 
} // fin main