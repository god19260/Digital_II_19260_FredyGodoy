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
unsigned char CNY70 = 0;                //sensor de color
unsigned char env_CNY70 = 0;            //dato enviado al master, 1 = rosado, 0 = no rosado
unsigned char Con_CNY70 = 0;            //contador para asegurar que sea rosado
unsigned char UltraFlag = 0;            //controlador de la lectura del ultrasónico
unsigned char arriba_abajo = 0;         //hacia arriba = 1, Hacia abajo = 0
unsigned char arriba_abajo_enable = 0;  //bandera para parar los motores
unsigned char env_master= 0;            // 1: inicio = 1; 2: enviar Ultra; 3: enviar CNY70
unsigned char z = 0;                    // lectura subbufer
unsigned char prueba = 50;              // lectura subbufer
unsigned char inicio = 1;               //utilizado para secuencia de inicio
unsigned char erguido = 1;      
unsigned char cont_miedo = 0;           //tiempo de espera para esconderse
char Distancia = 0;
//--------------------- Prototipo función configuración ------------------------
void config(void);
char Ultrasonico(void);

//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
      //Interrupcion del ADC module
    if (ADIF == 1){
        //Lectura del CNY70
        ADIF = 0;
        CNY70 = Valor_ADC(0); 
        if  (CNY70 > 55 && CNY70 < 70){     //Rango calculado en base experimentación del color rosado
            Con_CNY70++;
            if (Con_CNY70 == 254){          //detectar durante varios segundos que es rosado para evitar colores parecidos
                Con_CNY70 = 253;
            }
        }else {
            Con_CNY70 = 0;
        }
    } // Fin Interrupcion ADC
    
    // Inicio Interrupcion I2C
    if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (env_master == 0){
                env_master = 1;
                SSPBUF = Distancia;
            } else {
                env_master = 0;
                SSPBUF = env_CNY70;
            }
            PORTBbits.RB4 = ~PORTBbits.RB4;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    Config_Oscilador(); 
    Config_ADC();
    I2C_Slave_Init(0b11111110);     //dirección Slave II
    __delay_ms(5);
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){  
        if  (CNY70 > 55 && CNY70 < 70 && Con_CNY70 >= 200 && inicio == 0 && erguido == 1){ // detectar rosado
            UltraFlag = 1;
            arriba_abajo = 0;       //baja por miedo
            arriba_abajo_enable = 1;
            env_CNY70 = 1;
            RB5 = 1;
            erguido = 0;
            __delay_ms(36);
            while (RB6 == 1);
            
        }
        
        if (inicio == 1 ){          //secuencia de inicio
            arriba_abajo = 1;
            UltraFlag = 1;
            arriba_abajo_enable = 1;
            RB5 = 1;
            erguido = 0;
            __delay_ms(36);
            while (RB6 == 1);       //esperar a que el animatrónico este erguido
        }
        
        if (cont_miedo == 1){
            __delay_ms(2500);
            arriba_abajo = 1;       //sube luego de 2.5s
            UltraFlag = 1;
            arriba_abajo_enable = 1;
            cont_miedo = 0;
        }
        
        if (UltraFlag == 1){ 
           Distancia = Ultrasonico();   //lectura del ultrasonico
           //Control de banderas
           if (Distancia < 6 && arriba_abajo == 0){             //hasta abajo
               arriba_abajo_enable = 0;
               UltraFlag = 0;
               cont_miedo = 1;          //inicio de secuencia para volver a subir
               env_CNY70 = 0;
               
               
           }else if (Distancia > 15 && arriba_abajo == 1){      //hasta arriba
               arriba_abajo_enable = 0;
               inicio = 0;
               UltraFlag = 0;
               cont_miedo = 0;
               RB5 = 0;                 //avisarle al animatrónico que ya puede moverse de nuevo
               erguido = 1 ;
           }

           //Control de motores
           if (arriba_abajo == 1 && arriba_abajo_enable == 1 && PORTBbits.RB2 == 0){
               PORTBbits.RB1 = 1;       //hacer que el puente H  mueva los DC hacia arriba
           }else if (arriba_abajo == 0 && arriba_abajo_enable == 1 && PORTBbits.RB1 == 0){
               PORTBbits.RB2 = 1;       //hacer que el puente H mueva los DC hacia abajo
           }else{
               PORTBbits.RB1 = 0;
               PORTBbits.RB2 = 0;
           }
        }
        RB3 = ~RB3;                     //manera de visualizar si el pic no se traba
        __delay_ms(200);
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0b00000001;
    TRISB  = 0b01000000;
    TRISDbits.TRISD1 = 1;
    
    ANSEL  = 0b0000001;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTB = 0;
    PORTD = 0;
}
