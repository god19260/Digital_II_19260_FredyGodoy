/*
 * Proyecto I ESCLAVO I
 * File:   Proyecto_I.c 
 * Author: Fredy Godoy 19260
 *
 * Created on August 23, 2021, 9:07 AM
 */


#include <xc.h>
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
#define _XTAL_FREQ 8000000

char Valor_TMR0;
//valores de la diferencia entre la posici�n actual y su posici�n neutra
signed int DifServo1 = 0;
unsigned char DifServo1_Mayor5 = 0;
signed int DifServo2 = 0;
unsigned char DifServo2_Mayor5 = 0;
signed int DifServo3 = 0;
unsigned char DifServo3_Mayor5 = 0;
signed int DifServo4 = 0;
unsigned char DifServo4_Mayor5 = 0;
signed int DifServo5 = 0;
unsigned char DifServo5_Mayor5 = 0;
signed int DifServo6 = 0;
unsigned char DifServo6_Mayor5 = 0;
signed char menos_uno = -1;

//posici�n neutra de servos para que este erguido
unsigned char Pos_neutra_Servo1 = 210;
unsigned char Pos_neutra_Servo2 = 220;
unsigned char Pos_neutra_Servo3 = 220;
unsigned char Pos_neutra_Servo4 = 214;
unsigned char Pos_neutra_Servo5 = 220;
unsigned char Pos_neutra_Servo6 = 204;
unsigned char Erguido = 0;

//Control de la posici�n de los servos y sus l�mites
char Pos_Servos1 = 210; // 43-212
char Pos_Servos2 = 220; // 40-225
char Pos_Servos3 = 220; // 40-225
char Pos_Servos4 = 214; // 49-217
char Pos_Servos5 = 220; // 40-225
char Pos_Servos6 = 204; // 49-217

unsigned char izq_der = 3;  //0 = der/izq, 1 = izq/der
unsigned char izq_der_old = 0;  
unsigned char z = 0;
unsigned char prueba = 50;
//--------------------- Prototipo funci�n configuraci�n ------------------------
void config(void);


//------------------------------------------------------------------------------
//*************************** Interrupciones ***********************************
void __interrupt() isr (void){ 
// Interrupcion del timer0
    if (T0IF == 1){
        // Interrupcion cada 20ms: tmr0 100, prescaler 256, 8MHz de oscilador
        T0IF = 0;
        TMR0 = Valor_TMR0;
        // Se�al PWM
        // SERVO 1
        RD0 = 1;
        for(int i = 0; i<Pos_Servos1;i++){}
        RD0=0; 
        // SERVO 2
        RD1 = 1;
        for(int i = 0; i<Pos_Servos2;i++){}
        RD1=0; 
        // SERVO 3
        RD2 = 1;
        for(int i = 0; i<Pos_Servos3;i++){}
        RD2=0; 
        // SERVO 4
        RD3 = 1;
        for(int i = 0; i<Pos_Servos4;i++){}
        RD3=0; 
        // SERVO 5
        RD4 = 1;
        for(int i = 0; i<Pos_Servos5;i++){}
        RD4=0; 
        // SERVO 6
        RD5 = 1;
        for(int i = 0; i<Pos_Servos6;i++){}
        RD5=0; 
    } // Fin de interrupci�n timer0    
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
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            PORTBbits.RB1 = 1;
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = prueba;
            PORTBbits.RB1 = ~PORTBbits.RB1;
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
    config(); // Configuraci�n del progama
    
    Config_Oscilador();
    Valor_TMR0 = Config_TMR0();
    I2C_Slave_Init(0x70);           //direcci�n del slave I
    
//------------------------------------------------------------------------------
//*************************** loop principal ***********************************   
    while(1){ 
        if(RB3 == 1){               //control si se desea bajar el animatronico para que se coloque en erguido
            if(izq_der == 0 || izq_der == 2){
                izq_der = 3;        //modo de colocaci�n en erguido
                RB7 = 1;
            }else if (izq_der == 3){
                if (Erguido == 1){
                    RB7 = 0;        //indicaci�n al Slave II que ya se ha erguido 
                }
            }
        }

        
        if (izq_der == 0){          //secuencia de serpenteo 1
            //Movimiento de servos paso a paso hasta llegar a la posici�n deseada
            //servo arriba
            if (Pos_Servos1 < 212 && Pos_Servos5 == 195){
                Pos_Servos1++;
            }
            if (Pos_Servos4 > 50 && Pos_Servos5 == 195){
                Pos_Servos4 = Pos_Servos4 - 2;
            }
            if (Pos_Servos6 < 216 && Pos_Servos5 == 195){
                Pos_Servos6 = Pos_Servos6 + 2;
            }
            
            //servos abajo
            if (Pos_Servos2 < 225){
                Pos_Servos2++;
            }
            if (Pos_Servos3 < 225){
                Pos_Servos3++;
            }
            if (Pos_Servos5 > 195){
                Pos_Servos5--;
            }
            
            //comparaci�n si ya se ha llegado a su posici�n final
            if (Pos_Servos1 == 212 && Pos_Servos2 == 225 && Pos_Servos3 == 225 && Pos_Servos4 == 50 && Pos_Servos5 == 195 && Pos_Servos6 == 216){
                izq_der = 1;
                izq_der_old = 0;
            }
            __delay_ms(35);
        
        }else if (izq_der == 1){        //Modo para ponerse erguido
            //se busca cual es el servo que esta m�s lejos de su posici�n neutra
            //para que sea el que se mueva m�s r�pido y primero
            DifServo1 = Pos_neutra_Servo1 - Pos_Servos1;
            if (DifServo1 < 0){
                DifServo1 = DifServo1 * menos_uno;
            }if (DifServo1 < 5){
                DifServo1_Mayor5 = 1;
            }

            DifServo2 = Pos_neutra_Servo2 - Pos_Servos2;
            if (DifServo2 < 0){
                DifServo2 = DifServo2 * menos_uno;
            }if (DifServo2 < 5){
                DifServo2_Mayor5 = 1;
            }

            DifServo3 = Pos_neutra_Servo3 - Pos_Servos3;
            if (DifServo3 < 0){
                DifServo3 = DifServo3 * menos_uno;
            }if (DifServo3 < 5){
                DifServo3_Mayor5 = 1;
            }

            DifServo4 = Pos_neutra_Servo4 - Pos_Servos4;
            if (DifServo4 < 0){
                DifServo4 = DifServo4 * menos_uno;
            }if (DifServo4 < 5){
                DifServo4_Mayor5 = 1;
            }

            DifServo5 = Pos_neutra_Servo5 - Pos_Servos5;
            if (DifServo5 < 0){
                DifServo5 = DifServo5 * menos_uno;
            }if (DifServo5 < 5){
                DifServo5_Mayor5 = 1;
            }

            DifServo6 = Pos_neutra_Servo6 - Pos_Servos6;
            if (DifServo6 < 0){
                DifServo6 = DifServo6 * menos_uno;
            }if (DifServo6 < 5){
                DifServo6_Mayor5 = 1;
            }
            
            //Comparaci�n de cu�l es el m�s lejano y que se mueva m�s pasos
            if (DifServo1 > DifServo2 && DifServo1 > DifServo3 && DifServo1 > DifServo4 && DifServo1 > DifServo5 && DifServo1 > DifServo6 && DifServo1_Mayor5 == 0){
                if (Pos_Servos1 < Pos_neutra_Servo1){
                    Pos_Servos1 = Pos_Servos1 + 4;
                }else if (Pos_Servos1 > Pos_neutra_Servo1){
                    Pos_Servos1 = Pos_Servos1 - 4;
                }
            }else if (DifServo2 > DifServo1 && DifServo2 > DifServo3 && DifServo2 > DifServo4 && DifServo2 > DifServo5 && DifServo2 > DifServo6 && DifServo2_Mayor5 == 0){
                if (Pos_Servos2 < Pos_neutra_Servo2){
                    Pos_Servos2 = Pos_Servos2 + 4;
                }else if (Pos_Servos2 > Pos_neutra_Servo2){
                    Pos_Servos2 = Pos_Servos2 - 4;
                }
            }else if (DifServo3 > DifServo1 && DifServo3 > DifServo2 && DifServo3 > DifServo4 && DifServo3 > DifServo5 && DifServo3 > DifServo6 && DifServo3_Mayor5 == 0){
                if (Pos_Servos3 < Pos_neutra_Servo3){
                    Pos_Servos3 = Pos_Servos3 + 4;
                }else if (Pos_Servos3 > Pos_neutra_Servo3){
                    Pos_Servos3 = Pos_Servos3 - 4;
                }
            }else if (DifServo4 > DifServo1 && DifServo4 > DifServo2 && DifServo4 > DifServo3 && DifServo4 > DifServo5 && DifServo4 > DifServo6 && DifServo4_Mayor5 == 0){
                if (Pos_Servos4 < Pos_neutra_Servo4){
                    Pos_Servos4 = Pos_Servos4 + 4;
                }else if (Pos_Servos4 > Pos_neutra_Servo4){
                    Pos_Servos4 = Pos_Servos4 - 4;
                }
            }else if (DifServo5 > DifServo1 && DifServo5 > DifServo2 && DifServo5 > DifServo3 && DifServo5 > DifServo4 && DifServo5 > DifServo6 && DifServo5_Mayor5 == 0){
                if (Pos_Servos5 < Pos_neutra_Servo5){
                    Pos_Servos5 = Pos_Servos5 + 4;
                }else if (Pos_Servos5 > Pos_neutra_Servo5){
                    Pos_Servos5 = Pos_Servos5 - 4;
                }
            }else if (DifServo6 > DifServo1 && DifServo6 > DifServo2 && DifServo6 > DifServo3 && DifServo6 > DifServo4 && DifServo6 > DifServo5 && DifServo6_Mayor5 == 0){
                if (Pos_Servos6 < Pos_neutra_Servo6){
                    Pos_Servos6 = Pos_Servos6 + 4;
                }else if (Pos_Servos6 > Pos_neutra_Servo6){
                    Pos_Servos6 = Pos_Servos6 - 4;
                }
            }
            
            //Movimiento de todos los servos hacia su posici�n neutral
            if (Pos_Servos1 != Pos_neutra_Servo1){
                if (Pos_Servos1 < Pos_neutra_Servo1){
                    Pos_Servos1++;
                }else if (Pos_Servos1 > Pos_neutra_Servo1){
                    Pos_Servos1--;
                }
            }
            if (Pos_Servos2 != Pos_neutra_Servo2){
                if (Pos_Servos2 < Pos_neutra_Servo2){
                    Pos_Servos2++;
                }else if (Pos_Servos2 > Pos_neutra_Servo2){
                    Pos_Servos2--;
                }
            }
            if (Pos_Servos3 != Pos_neutra_Servo3){
                if (Pos_Servos3 < Pos_neutra_Servo3){
                    Pos_Servos3++;
                }else if (Pos_Servos3 > Pos_neutra_Servo3){
                    Pos_Servos3--;
                }
            }
            if (Pos_Servos4 != Pos_neutra_Servo4){
                if (Pos_Servos4 < Pos_neutra_Servo4){
                    Pos_Servos4++;
                }else if (Pos_Servos4 > Pos_neutra_Servo4){
                    Pos_Servos4--;
                }
            }
            if (Pos_Servos5 != Pos_neutra_Servo5){
                if (Pos_Servos5 < Pos_neutra_Servo5){
                    Pos_Servos5++;
                }else if (Pos_Servos5 > Pos_neutra_Servo5){
                    Pos_Servos5--;
                }
            }
            if (Pos_Servos6 != Pos_neutra_Servo6){
                if (Pos_Servos6 < Pos_neutra_Servo6){
                    Pos_Servos6++;
                }else if (Pos_Servos6 > Pos_neutra_Servo6){
                    Pos_Servos6--;
                }
            }
            
            //Comparaci�n si todos los servos estan en su posici�n neutral
            if (Pos_Servos2 == Pos_neutra_Servo2 && Pos_Servos3 == Pos_neutra_Servo3 && Pos_Servos5 == Pos_neutra_Servo5 && Pos_Servos1 == Pos_neutra_Servo1 && Pos_Servos4 == Pos_neutra_Servo4 && Pos_Servos6 == Pos_neutra_Servo6){
                //selecci�n del siguiente modo
                if (izq_der < izq_der_old){
                    izq_der = 0;
                }else if (izq_der > izq_der_old){
                    izq_der = 2;
                }
                //si se desea bajar el animatr�nico obligar a que este en este modo
                if (RB3 == 1){
                izq_der = 3;
                }
                //izq_der = 0;
            }
            __delay_ms(35);
        }else if (izq_der == 2){            //secuencia  de serpenteo 2
            //Movimiento de los servos hacia la posici�n deseada paso a paso
            //Servos de arriba
            if (Pos_Servos1 < 212 && Pos_Servos3 == 160){
                Pos_Servos1++;
            }
            if (Pos_Servos4 < 216 && Pos_Servos3 == 160){
                Pos_Servos4 = Pos_Servos4 + 2;
            }
            if (Pos_Servos6 > 50 && Pos_Servos3 == 160){
                Pos_Servos6 = Pos_Servos6 - 2;
            }
            
            //Servos de abajo
            if (Pos_Servos2 < 225){
                Pos_Servos2++;
            }
            if (Pos_Servos3 > 160){
                Pos_Servos3--;
            }
            if (Pos_Servos5 < 225){
                Pos_Servos5++;
            }
            
            //comparaci�n si los servos llegaron a la posici�n deseada
            if (Pos_Servos1 == 212 && Pos_Servos2 == 225 && Pos_Servos3 == 160 && Pos_Servos4 == 216 && Pos_Servos5 == 225 && Pos_Servos6 == 50){
                izq_der = 1;
                izq_der_old = 2;
            }
            __delay_ms(35);
        }else if (izq_der == 3){          //modo bajar el animatr�nico
            //luego que la posici�n este en erguido, debido a la parte mec�ncia se requiere
            //de erguir m�s la parte de abajodel animatr�nico
            if(Pos_Servos2 > 195){
            Pos_Servos2--;
            }
            if(Pos_Servos3 > 195){
            Pos_Servos3--;
            }
            if(Pos_Servos5 > 195){
            Pos_Servos5--;
            }
            
            //bandera para indicar que ya puede bajarse el animatr�nico
            if (Pos_Servos2 == 195 && Pos_Servos3 == 195 && Pos_Servos5 == 195){
                Erguido = 1;
            }
            //bandera en la que ya no se va a bajar o a subir el animatr�nico
            //por lo tanto puede moverse como las secuencias
            if (RB3 == 0){
                izq_der = 0;
                Erguido = 0;
            }
            __delay_ms(35);
        }
         
    } // fin loop principal while 
} // fin main

void config(void){    
    TRISD  = 0;
    TRISB  = 0b00001000;
    
    ANSEL  = 0;
    ANSELH = 0;
    
    //Limpieza de puertos
    PORTD = 0;  
    PORTB = 0;  
}


