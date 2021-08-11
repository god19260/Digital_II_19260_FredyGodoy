/*
 * File:   Lab_04_Esclavo_2.c
 * Author: fredy
 *
 * Created on August 9, 2021, 5:30 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LIB.h"
#include "../../LIB/LIB.X/I2C.h"


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
char z;
//--------------------- Prototipo función configuración ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Inicio Interrupcion I2C
    if(PIR1bits.SSPIF == 1){  

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) { //Lectura 
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            // SSPBUF
   
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){ // Escritura
            z = SSPBUF;
            BF = 0;
            // SSPBUF
            SSPBUF = PORTA;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    } // Fin interrupcion I2C
    
    // Interrupcion del PORTB
    if (RBIF == 1){
        if(RB0 == 0){
            Boton_0 = 1;
        }
        if(RB1 == 0){
            Boton_1 = 1;
        }
        RBIF = 0; // Apagar bandera del puerto B
    }// Fin de interrupción del PORTB    
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    
    Config_Oscilador();
    Config_PORTB();
    I2C_Slave_Init(0x60);
    

//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){      
        if (Boton_0 == 1){
            if(PORTA == 0b00001111){
                PORTA = 0x00;
            }else{
                PORTA++;
            }
            Boton_0 = 0;
        }else if(Boton_1 == 1){
            if(PORTA == 0x00){
                PORTA = 0b00001111;
            }else{
                PORTA--;    
            }
            Boton_1 = 0;
        }
        //PORTD = PORTD && 0b11111110;

    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0;
    TRISE  = 0;
    TRISD  = 0;
    
    ANSEL  = 0;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    //PORTB = 0;
    PORTC = 0;
    PORTD = 0xff;
    PORTE = 0;
}