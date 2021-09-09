/*
 * Proyecto I MAESTRO
 * File:   Proyecto_I.c
 * Author: Fredy Godoy 19260
 *
 * Created on August 23, 2021, 9:07 AM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#include "../../LIB/LIB.X/LCD.h"
#include "../../LIB/LIB.X/LIB.h"
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
unsigned char second = 0;
unsigned char minute = 0x41;
unsigned char hour   = 0x70;
unsigned char hourbits   = 0;
unsigned char AM_PM   = 0;
unsigned char day    = 0x05;
unsigned char date   = 0x04;
unsigned char month  = 0x09; 
unsigned char year   = 0x21;

unsigned char hora_sensores = 1;
unsigned char hora_sensores_old = 1;
unsigned char ultrasonico = 0;
unsigned char CNY70 = 0;
unsigned char contador = 0;
unsigned char Servos = 0;
char text [16];
char ASCII [] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
//--------------------- Prototipo función configuración ------------------------
void config(void);
void convdecimal (unsigned char valor);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    
    if (PIR1bits.RCIF){
        hora_sensores_old = hora_sensores; 
        if (RCREG == '1'){                  //observar hora
            hora_sensores = 1; 
        } else if (RCREG == '0'){           //observar sensores
            hora_sensores = 0; 
        }
        RCIF = 0;
    }
}
       

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************        
    config(); // Configuración del progama
    Config_Oscilador();
    Lcd_Init();
    Config_USART();
    I2C_Master_Init(100000);                // Inicializar Comuncación I2C
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;

    //CONFIGURACIÓN DEL RTC, de esta matera se programo la hora inicial del RTC
//    __delay_ms(500);
//    I2C_Master_Start                      // Start I2C protocol
//    I2C_Master_Write(0xD0);               // DS3231 address
//    I2C_Master_Write(second);             // Send register address (seconds address)
//    I2C_Master_Write(0);                  // Reset seconds and start oscillator
//    I2C_Master_Write(minute);             // Write minute value to DS3231
//    I2C_Master_Write(hour);               // Write hour value to DS3231
//    I2C_Master_Write(day);                // Write day value
//    I2C_Master_Write(date);               // Write date value to DS3231
//    I2C_Master_Write(month);              // Write month value to DS3231
//    I2C_Master_Write(year);               // Write year value to DS3231
//    I2C_Master_Stop                       // Stop I2C
//    __delay_ms(200);                      // Wait 200ms
    
//-------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){
        //Comunicación con Slave I (servos)
        I2C_Master_Start();
        I2C_Master_Write(0x71);
        Servos = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(100);
        
        //Comunicación con Slave II (sensores), pedida del valor del ultrasónico
        I2C_Master_Start();                                
        I2C_Master_Write(0b11111111);                           
        ultrasonico = I2C_Master_Read(0);                  
        I2C_Master_Stop();
        __delay_ms(100);
        //Comunicación con Slave II (sensores), pedida del valor del CNY70
        I2C_Master_Start();                                
        I2C_Master_Write(0b11111111);                            
        CNY70 = I2C_Master_Read(0);                  
        I2C_Master_Stop();
        __delay_ms(100);
        
        //Comunicación con RTC
        I2C_Master_Start();                                   
        I2C_Master_Write(0xD0);             // dirección del RTC
        I2C_Master_Write(0);                // se coloca el mapa del RTC en 0 (segundos)           
        I2C_Master_Start();                 // reinicio de comunicación I2C
        I2C_Master_Write(0xD1);             // Modo lectura
        second = I2C_Master_Read(1);        // se lee segundos
        minute = I2C_Master_Read(1);        // Se puede seguir leyendo ya que el RTC posee un puntuero que
        hour   = I2C_Master_Read(1);        // Aumenta en luego de escribirle o leer en una dirección
        day    = I2C_Master_Read(1);                          
        date   = I2C_Master_Read(1);                          
        month  = I2C_Master_Read(1);                          
        year   = I2C_Master_Read(0);                          
        I2C_Master_Stop();                  // Stop I2C protocol
        __delay_ms(100);


        // Conversición de datos según el manual del RTC
        second = (second >> 4) * 10 + (second & 0x0F);
        minute = (minute >> 4) * 10 + (minute & 0x0F);
        hourbits = hour;
        hour = (hour >> 4 & 0b0001) * 10 + (hour & 0x0F);
        date = (date >> 4) * 10 + (date & 0x0F);
        month = (month >> 4) * 10 + (month & 0x0F);
        year = (year >> 4) * 10 + (year & 0x0F);

        

        //selección de lo que se desplegará en la LCD
        if (hora_sensores == 1){
            if(hora_sensores_old == 0 && hora_sensores == 1){
                Lcd_Clear();
                hora_sensores_old = 1;
            }
            
            __delay_ms(10);
            Lcd_Set_Cursor(1,4);            //Fila 1: Hora:minuto:segundo   
            convdecimal (hour);
            Lcd_Write_Char(':');
            convdecimal (minute);
            Lcd_Write_Char(':');
            convdecimal (second);
            Lcd_Set_Cursor(1,13);           //AM/PM 
            AM_PM = hourbits >> 5;
            if (AM_PM == 3){
                Lcd_Write_String("PM");
            }else if (AM_PM == 2){
                Lcd_Write_String("AM");
            }

            __delay_ms(10);

            Lcd_Set_Cursor(2,4);            //Fila 2: fecha/mes/año
            convdecimal (date);
            Lcd_Write_Char('/');
            convdecimal (month);
            Lcd_Write_Char('/');
            Lcd_Write_Char('2');
            Lcd_Write_Char('0');
            convdecimal (year);

        }else if (hora_sensores == 0){
            Lcd_Set_Cursor(1,1);            //Fila 1: Ultrasónico: valor
            Lcd_Write_String("Ultrasonico: ");
            convdecimal (ultrasonico);
            Lcd_Set_Cursor(2,1);            //Fila 2: Color Rosado: Si/NO
            Lcd_Write_String("Color Rosado: ");
            if (CNY70 == 1){
                Lcd_Write_String("SI");
            }else if (CNY70 == 0){
                Lcd_Write_String("NO");
            }

        }

        //envío de datos a Adafruit en donde se separa la string entera por medio de los espacios entre cada dato
        sprintf(text,"%d %d %d:%d",ultrasonico,CNY70,hour, minute);
        Texto_USART(text);
        RD1 = ~RD1;
        
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISA  = 0;
    TRISD  = 0;
    TRISE  = 0;
    
    ANSEL  = 0;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;
}
void convdecimal (unsigned char valor){     //envio a la LCD en 2 digitos de los datos
    unsigned char decena;
    unsigned char unidad; 
        decena = valor / 10;
        unidad = valor % 10;
        Lcd_Write_Char(ASCII[decena]);
        Lcd_Write_Char(ASCII[unidad]);  
}
