/*
 * Universidad del Valle de Guatemala
 * Digital II
 * File:   Lab_0.c
 * PIC16F887
 * Author: Fredy Godoy
 * Carne: 19260
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
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
struct{
    char bit0;
    char bit1;
    char bit2;
}Comparador_PORTB;
unsigned char Boton_0 = 0;
unsigned char Boton_1 = 0;
unsigned char Boton_2 = 0;
unsigned char Contador_J1 = 0;
unsigned char Contador_J2 = 0;
unsigned char Juego_Iniciado = 0;
//------------------------------------------------------------------------------
//***************************** Prototipos *************************************
char Tabla_Display (char numero);
void Secuencia_Inicio(void);
void Botones(void);
//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
    // Interrupcion del PORTB
    if (RBIF == 1){
        if(RB0 == 0 && Comparador_PORTB.bit0 == 1){
            Boton_0 = 1;
        }
        if(RB1 == 0 && Comparador_PORTB.bit1 == 1 && Boton_0 == 0){
            Boton_1 = 1;
        }
        if(RB2 == 0 && Comparador_PORTB.bit2 == 1 && Boton_0 == 0){ 
            Boton_2 = 1;
        }
        RBIF = 0; // Apagar bandera del puerto B
    }// Fin de interrupción del PORTB
}    

void main(void) {
//------------------------------------------------------------------------------
//*************************** Configuraciones **********************************    
    // Oscilador
    IRCF0 = 1;       // Configuración del reloj interno 
    IRCF1 = 1;
    IRCF2 = 1;       // 8 Mhz   
    INTCON = 0b11101000;
    // Configuración del puerto B
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0=1;
    WPUBbits.WPUB1=1;
    WPUBbits.WPUB2=1;
    // Activación Interrup on change
    IOCB0 = 1;
    IOCB1 = 1;
    IOCB2 = 1;
    
    // Configurar puertos
    ANSEL  = 0;
    ANSELH = 0;
    TRISA  = 0;          // Definir el puerto A como salida
    TRISB  = 0b00111111; // Definir como salidas los pines 6 y 7 del puerto B
    TRISC  = 0;          // Definir el puerto C como salida
    TRISD  = 0;          // Definir el puerto D como salida
    TRISE  = 0;          // Definir el puerto E como salida
    
    //Limpieza de puertos
    PORTA = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

//------------------------------------------------------------------------------
//*************************** loop principal ***********************************
    while(1){
        Botones();
        //PORTA = Tabla_Display(1);
    } // fin loop principal while 
} // fin main

//------------------------------------------------------------------------------
//***************************** Funciones  *************************************
char Tabla_Display (char numero){
    char tabla [16]= {
        0b0111111, // Cero
        0b0000110, // Uno
        0b1011011, // Dos
        0b1001111, // Tres
        0b1100110, // Cuatro
        0b1101101, // Cinco
        0b1111101, // Seis 
        0b0000111, // Siete
        0b1111111, // Ocho
        0b1100111, // Nueve
        0b1110111, // A
        0b1111100, // B
        0b0111001, // C
        0b1011110, // D
        0b1111001, // E
        0b1110001, // F
    }; 
    return tabla[numero];
}

void Secuencia_Inicio(void){
    // Color Rojo, 3 seg
    RE0 = 1;
    RE1 = 0;
    RE2 = 0;
    PORTA = Tabla_Display(3);
    __delay_ms(1000);
    // Color Amarillo, 2 seg
    RE0 = 0;
    RE1 = 1;
    RE2 = 0;
    PORTA = Tabla_Display(2);
    __delay_ms(1000);
    // Color Verde, 1 seg
    RE0 = 0;
    RE1 = 0;
    RE2 = 1;
    PORTA = Tabla_Display(1); 
    // 0 seg
    __delay_ms(1000);
    RE0 = 0;
    RE1 = 0;
    RE2 = 0;
    PORTA = Tabla_Display(0); 
}

void Botones(void){
    if(Boton_0 == 1){
        RB6 = 0;
        RB7 = 0;
        PORTC = 0;
        PORTD = 0;
        Secuencia_Inicio();
        Juego_Iniciado = 1;
        Boton_0 = 0; // Apagar la bandera del boton
        Boton_1 = 0; // Apagar la bandera del boton
        Boton_2 = 0; // Apagar la bandera del boton
    }
    if (RB6 == 0 && RB7 == 0 && Juego_Iniciado == 1){
        if(Boton_1 == 1 && RB6 == 0){
            if (PORTC == 128){
                PORTA = Tabla_Display(1);
                RB6 = 1;
                PORTC = 0;
            } else{
                if (PORTC == 0){
                    PORTC = 1;
                }else{
                    PORTC = PORTC * 2;
                }    
            }
            Boton_1 = 0; // Apagar la bandera del boton
        }
        if(Boton_2 == 1 && RB7 == 0){
            if (PORTD == 128){
                PORTA = Tabla_Display(2);
                RB7 = 1;
                PORTD = 0;
            } else{
                if (PORTD == 0){
                    PORTD = 1;
                }else{
                    PORTD = PORTD * 2;
                }
            }
            Boton_2 = 0; // Apagar la bandera del boton
        }    
    }
    
    Comparador_PORTB.bit0 = RB0;  // Guardar el estado actual del boton
    Comparador_PORTB.bit1 = RB1;  // Guardar el estado actual del boton
    Comparador_PORTB.bit2 = RB2;  // Guardar el estado actual del boton
}