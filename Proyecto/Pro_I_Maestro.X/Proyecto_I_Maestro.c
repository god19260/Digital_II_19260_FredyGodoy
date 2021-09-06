/*
 * Proyecto I MAESTRO
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
char temp;
char Seg; 
char Min;
char Hora; 
char Dia;
char Fecha;
char Mes;
char Year;
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
    Config_USART();
    LCD_Init_8bits();
    I2C_Master_Init(100000);     // Inicializar Comuncación I2C
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){   
        // Sensor I2C - RTC
        I2C_Master_Start();
        I2C_Master_Write(0xD0); // Direccion Sensor I2C - RTC
        I2C_Master_Write(0);        // comando para segundos
        I2C_Master_Start();
        I2C_Master_Write(0xD1);
        Seg    = I2C_Master_Read(1);       // Read seconds from register 0
        Min    = I2C_Master_Read(1);       // Read minutes from register 1
        Hora   = I2C_Master_Read(0);       // Read hour from register 2
//        Dia    = I2C_Master_Read(1);       // Read day from register 3
//        Fecha  = I2C_Master_Read(1);       // Read date from register 4
//        Mes    = I2C_Master_Read(1);       // Read month from register 5
//        Year   = I2C_Master_Read(0);       // Read year from register 6
        I2C_Master_Stop();                 // Stop I2C protocol
        
        Seg = (Seg>>4) * 10 + (Seg & 0x0f);
        Min = (Min>>4) * 10 + (Min & 0x0f);
        Hora = (Hora>>4) * 10 + (Hora & 0x0f);
        sprintf(text, "%d:%d:%d",Hora, Min, Seg);
        Texto_USART(text);
        
        // -------- LCD ---------
        Lcd_Set_Cursor(1,1);
        Write_LCD(text);
        RD1 = ~RD1;
        __delay_ms(200);
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
