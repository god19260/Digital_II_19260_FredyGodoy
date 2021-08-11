/*
 * File:   Lab_04_Maestro.c
 * Author: Fredy Godoy 
 *
 * Created on August 9, 2021, 5:28 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LIB.h"
#include "../../LIB/LIB.X/I2C.h"
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
unsigned char V_ADC_0 = 0;
unsigned char Cont_Slave_II;
unsigned char temp;
unsigned int Sen_MS;
unsigned int Sen_LS;
unsigned int Sen_Total;
unsigned char Sen_Checksum;
double Temperatura;
char text[16];
//--------------------- Prototipo función configuración ------------------------
void config(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
//void __interrupt() isr (void){ 
//    
//}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    
    Config_Oscilador();
    LCD_Init_8bits();
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C

//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){            
        // Lectura Primer Esclavo
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        PORTD = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);

        // Dispositivo I2C (temp) - Hold
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        Sen_MS = I2C_Master_Read(0);
        Sen_LS = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        // Conversion de datos
        Sen_Total = Sen_MS<<8;
        Sen_Total += Sen_MS;
        Sen_Total &= ~0b11;
        Temperatura = -46.85 +(175.72*Sen_Total/65536);
        sprintf(text, "%.0f", Temperatura);
        
        // Lectura Segundo Esclavo
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        Cont_Slave_II = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        PORTB = Cont_Slave_II;

        
        //------ LCD -------
        Lcd_Set_Cursor(1,1);
        Write_LCD("S1:   S2:    S3:");
        Lcd_Set_Cursor(2,1);
        temp = PORTD;
        Print_Cont(temp);
        Lcd_Set_Cursor(2,7);
        temp = PORTB;
        Print_Cont(temp);
        RE2 = ~RE2;
        Lcd_Set_Cursor(2,13);
        Write_LCD(text);
        Write_LCD("C");
         
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
