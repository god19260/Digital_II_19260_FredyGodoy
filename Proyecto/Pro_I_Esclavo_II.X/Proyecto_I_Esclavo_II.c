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
#include "../../LIB/LIB.X/I2C.h"
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
unsigned char env_CNY70 = 0;
unsigned char Con_CNY70 = 0;
unsigned char UltraFlag = 0;
unsigned char arriba_abajo = 0; //hacia arriba = 1, Hacia abajo = 0
unsigned char arriba_abajo_flag = 0; 
unsigned char arriba_abajo_enable = 0; //hacia arriba = 1, Hacia abajo = 0
unsigned char env_master= 0; // 1: inicio = 1; 2: enviar Ultra; 3: enviar CNY70
unsigned char z = 0;        // lectura subbufer
unsigned char inicio = 2; 
unsigned char cont_miedo = 0; 
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
        if  (CNY70 > 40 && CNY70 < 55){
            Con_CNY70++;
        }else {
            Con_CNY70 = 0;
        }
    } // Fin Interrupcion ADC
    
    // Inicio Interrupcion I2C
    if(PIR1bits.SSPIF == 1){  

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;  
                       // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) { //Lectura 
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            inicio =1;
            env_master = 1;  
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
            if (env_master == 0)
                env_master = 1;
            else if (env_master == 1)
                env_master = 0;
        
            if (env_master == 1){
                SSPBUF = Distancia;
            }else if (env_master == 0){
                SSPBUF = env_CNY70;
            }
            
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       

        
        PIR1bits.SSPIF = 0;
    } // Fin interrupcion I2C
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    Config_Oscilador(); 
    Config_ADC();
    I2C_Slave_Init(0b11111110);
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){  
        if  (CNY70 > 40 && CNY70 < 55 && Con_CNY70 >= 200 && inicio == 0){ // detectar rosado
            PORTBbits.RB0 = 1;
            UltraFlag = 1;
            arriba_abajo = 0;   //baja por miedo
            arriba_abajo_enable = 1;
            env_CNY70 = 1;
        }
        
        if (inicio == 1 ){      //secuencia de inicio
            arriba_abajo = 1;
            UltraFlag = 1;
            arriba_abajo_enable = 1;
        }
        if (cont_miedo == 1){
            __delay_ms(5000);
            arriba_abajo = 1;   //sube luego de 5s
            UltraFlag = 1;
            arriba_abajo_enable = 1;
        }
        
        if (UltraFlag == 1){ 
           Distancia = Ultrasonico();
           //Control de banderas
           if (Distancia < 6 && arriba_abajo == 0){
               arriba_abajo_enable = 0;
               UltraFlag = 0;
               cont_miedo = 1;
               env_CNY70 = 0;
           }else if (Distancia > 16 && arriba_abajo == 1){
               arriba_abajo_enable = 0;
               inicio = 0;
               UltraFlag = 0;
               cont_miedo = 0;
           }

           //Control de motores
           if (arriba_abajo == 1 && arriba_abajo_enable == 1 && PORTBbits.RB2 == 0){
               PORTBbits.RB1 = 1;
           }else if (arriba_abajo == 0 && arriba_abajo_enable == 1 && PORTBbits.RB1 == 0){
               PORTBbits.RB2 = 1;
           }else{
               PORTBbits.RB1 = 0;
               PORTBbits.RB2 = 0;
           }
        }
        RB4 = (inicio >>1) && 0x01;
        RB3 = ~RB3;  
        __delay_ms(200);
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0b00000001;
    TRISB  = 0;
    
    ANSEL  = 0b0000001;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    PORTB = 0;
}