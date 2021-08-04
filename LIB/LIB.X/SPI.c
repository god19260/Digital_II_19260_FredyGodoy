/* 
 * File: Libreria SPI 
 * Author: Fredy Godoy 19260
 * Comments: 
 * Revision history: 
 */

#include <xc.h>
#include "SPI.h"
#define _XTAL_FREQ 8000000

// Seleccionar slave
// Iniciar transmision de datos con master

void Init_Master(void){
    TRISC5 = 0;
    // Synchronous Serial Port Mode Select bits
    SSPCON = 0x00;
    SSPCONbits.SSPM0 = 0; 
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM3 = 0; // Fosc/4
    
    // Clock Polarity Select bit
    SSPCONbits.CKP = 0; // Idle state for clock is a low level
    
    // Synchronous Serial Port Enable bit
    SSPCONbits.SSPEN = 1; // Enables serial port and configures 
                          // SCK, SDO, SDI and SS
    
    SSPSTAT = 0x00;
    TRISC3 = 0; // Pin de salida CLK
    
}

void Init_Slave(void){
    // Interrupciones MSSP
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    
    TRISC5 = 0;
    // Synchronous Serial Port Mode Select bits
    SSPCON = 0x00;
    SSPCONbits.SSPM0 = 0; 
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM2 = 1;
    SSPCONbits.SSPM3 = 0; // SPI Slave mode, clock = SCK pin, SS pin control 
                          // enabled 
    
    // Synchronous Serial Port Enable bit
    SSPCONbits.SSPEN = 1; // Enables serial port and configures 
                          // SCK, SDO, SDI and SS
    
    SSPSTAT = 0x00;
    TRISC3 = 1; // Pin de entrada CLK
}

void spiWrite(char dat){
    // Escribir datos en bus de datos
    SSPBUF = dat;
}

char spiRead(){
    // Leer los datos recibidos
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
    __delay_ms(1);
    return(SSPBUF); // read the received data from the buffer
    
}

void spiReceiveWait(){
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}
