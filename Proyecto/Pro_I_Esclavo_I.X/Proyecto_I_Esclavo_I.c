/*
 * Proyecto I ESCLAVO I
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
char Valor_TMR0;
char text[16];
char V_Max_Servo = 220;
char V_Min_Servo = 49;
char Pos_Servo_1 = 91; // 49 - 220
char Pos_Servo_2 = 91;
char Pos_Servo_3 = 91;
char Pos_Servo_4 = 91;
char Pos_Servo_5 = 91;
char Pos_Servo_6 = 91;
//--------------------- Prototipo función configuración ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Interrupcion del timer0
    if (T0IF == 1){
        // Interrupcion cada 20ms: tmr0 100, prescaler 256, 8MHz de oscilador
        T0IF = 0;
        TMR0 = Valor_TMR0;
        // Señal PWM
        // SERVO 1
        RD0 = 1;
        for(int i = 0; i<Pos_Servo_1;i++){}
        RD0=0; 
        // SERVO 2
        RD1 = 1;
        for(int i = 0; i<Pos_Servo_2;i++){}
        RD1=0; 
        // SERVO 3
        RD2 = 1;
        for(int i = 0; i<Pos_Servo_3;i++){}
        RD2=0; 
        // SERVO 4
        RD3 = 1;
        for(int i = 0; i<Pos_Servo_4;i++){}
        RD3=0; 
        // SERVO 5
        RD4 = 1;
        for(int i = 0; i<Pos_Servo_5;i++){}
        RD4=0; 
        // SERVO 6
        RD5 = 1;
        for(int i = 0; i<Pos_Servo_6;i++){}
        RD5=0; 
    } // Fin de interrupción timer0   
    
    // Interrupcion USART
    if (RCIF == 1){
        // RCREG (Receptor)
        // TXREG (Transmisor)
        if (RCREG == '1'){ // servo 1
            if(Pos_Servo_1 <V_Max_Servo){
                Pos_Servo_1++;
                sprintf(text,"Servo 1: %d",Pos_Servo_1);
                Texto_USART(text);
            }else {
                Pos_Servo_1 = V_Min_Servo;
                sprintf(text,"Servo 1: %d",Pos_Servo_1);
                Texto_USART(text);
            }                        
        } else if(RCREG == '2'){
            if(Pos_Servo_1 > V_Min_Servo){
                Pos_Servo_1--;
                sprintf(text,"Servo 1: %d",Pos_Servo_1);
                Texto_USART(text);
            }else {
                Pos_Servo_1 = V_Max_Servo;
                sprintf(text,"Servo 1: %d",Pos_Servo_1);
                Texto_USART(text);
            } 
        }
        else if (RCREG == '3'){ // servo 2
            if(Pos_Servo_2 <V_Max_Servo){
                Pos_Servo_2++;
                sprintf(text,"Servo 2: %d",Pos_Servo_2);
                Texto_USART(text);
            }else {
                Pos_Servo_2 = V_Min_Servo;
                sprintf(text,"Servo 2: %d",Pos_Servo_2);
                Texto_USART(text);
            }                        
        } else if(RCREG == '4'){
            if(Pos_Servo_2 > V_Min_Servo){
                Pos_Servo_2--;
                sprintf(text,"Servo 2: %d",Pos_Servo_2);
                Texto_USART(text);
            }else {
                Pos_Servo_2 = V_Max_Servo;
                sprintf(text,"Servo 2: %d",Pos_Servo_2);
                Texto_USART(text);
            } 
        }
        else if (RCREG == '5'){ // servo 3
            if(Pos_Servo_3 <V_Max_Servo){
                Pos_Servo_3++;
                sprintf(text,"Servo 3: %d",Pos_Servo_3);
                Texto_USART(text);
            }else {
                Pos_Servo_3 = V_Min_Servo;
                sprintf(text,"Servo 3: %d",Pos_Servo_3);
                Texto_USART(text);                
            }                        
        } else if(RCREG == '6'){
            if(Pos_Servo_3 > V_Min_Servo){
                Pos_Servo_3--;
                sprintf(text,"Servo 3: %d",Pos_Servo_3);
                Texto_USART(text);
            }else {
                Pos_Servo_3 = V_Max_Servo;
                sprintf(text,"Servo 3: %d",Pos_Servo_3);
                Texto_USART(text);
            } 
        }
        else if (RCREG == '7'){ // servo 4
            if(Pos_Servo_4 <V_Max_Servo){
                Pos_Servo_4++;
                sprintf(text,"Servo 4: %d",Pos_Servo_4);
                Texto_USART(text);
            }else {
                Pos_Servo_4 = V_Min_Servo;
                sprintf(text,"Servo 4: %d",Pos_Servo_4);
                Texto_USART(text);
            }                        
        } else if(RCREG == '8'){
            if(Pos_Servo_4 > V_Min_Servo){
                Pos_Servo_4--;
                sprintf(text,"Servo 4: %d",Pos_Servo_4);
                Texto_USART(text);
            }else {
                Pos_Servo_4 = V_Max_Servo;
                sprintf(text,"Servo 4: %d",Pos_Servo_4);
                Texto_USART(text);
            } 
        }
        else if (RCREG == '9'){ // servo 5
            if(Pos_Servo_5 <V_Max_Servo){
                Pos_Servo_5++;
                sprintf(text,"Servo 5: %d",Pos_Servo_5);
                Texto_USART(text);
            }else {
                Pos_Servo_5 = V_Min_Servo;
                sprintf(text,"Servo 5: %d",Pos_Servo_5);
                Texto_USART(text);
            }                        
        } else if(RCREG == '0'){
            if(Pos_Servo_5 > V_Min_Servo){
                Pos_Servo_5--;
                sprintf(text,"Servo 5: %d",Pos_Servo_5);
                Texto_USART(text);
            }else {
                Pos_Servo_5 = V_Max_Servo;
                sprintf(text,"Servo 5: %d",Pos_Servo_5);
                Texto_USART(text);
            } 
        }
        else if (RCREG == 'q'){ // servo 6
            if(Pos_Servo_6 <V_Max_Servo){
                Pos_Servo_6++;
                sprintf(text,"Servo 6: %d",Pos_Servo_6);
                Texto_USART(text);
            }else {
                Pos_Servo_6 = V_Min_Servo;
                sprintf(text,"Servo 6: %d",Pos_Servo_6);
                Texto_USART(text);
            }                        
        } else if(RCREG == 'w'){
            if(Pos_Servo_6 > V_Min_Servo){
                Pos_Servo_6--;
                sprintf(text,"Servo 6: %d",Pos_Servo_6);
                Texto_USART(text);
            }else {
                Pos_Servo_6 = V_Max_Servo;
                sprintf(text,"Servo 6: %d",Pos_Servo_6);
                Texto_USART(text);
            } 
        }
        RCIF = 0; 
    } // Fin Interrupción USART
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    
    Config_Oscilador();
    Valor_TMR0 = Config_TMR0();
    Config_USART();
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){                
        __delay_ms(2000);
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0;
    TRISB  = 0;
    TRISE  = 0;
    TRISD  = 0;
    
    ANSEL  = 0;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    PORTB = 0;
   // PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}
