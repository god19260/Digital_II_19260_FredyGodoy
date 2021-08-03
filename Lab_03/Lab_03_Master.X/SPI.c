/* 
 * File: Libreria SPI 
 * Author: Fredy Godoy 19260
 * Comments: 
 * Revision history: 
 */

#include <xc.h>
#include "SPI.h"
#include "Libreria.h"
#define _XTAL_FREQ 8000000

// Seleccionar slave
// Iniciar transmision de datos con master

void Init_Master(void){
    // Synchronous Serial Port Mode Select bits
    SSPCONbits.SSPM0 = 0; 
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM3 = 0; // Fosc/4
    
    // Clock Polarity Select bit
    SSPCONbits.CKP = 1; // Idle state for clock is a high level
    
    // Synchronous Serial Port Enable bit
    SSPCONbits.SSPEN = 1; // Enables serial port and configures 
                          // SCK, SDO, SDI and SS
    
    // Sample bit
    SSPSTATbits.SMP = 1; //Input data sampled at end of data output time
    TRISC3 = 0; // Pin de salida CLK
    
    //SPI Clock Edge Select bit
    SSPSTATbits.CKE = 1;
    
}

void Init_Slave(void){
    // Synchronous Serial Port Mode Select bits
    SSPCONbits.SSPM0 = 1; 
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM2 = 1;
    SSPCONbits.SSPM3 = 0; // SPI Slave mode, clock = SCK pin, SS pin control 
                          // disabled, SS can be used as I/O pin
    
    // Clock Polarity Select bit
    SSPCONbits.CKP = 1; // Idle state for clock is a high level
    
    // Synchronous Serial Port Enable bit
    SSPCONbits.SSPEN = 1; // Enables serial port and configures 
                          // SCK, SDO, SDI and SS
    
    // Sample bit
    SSPSTATbits.SMP = 0;  // SMP must be cleared when SPI is used in Slave mode
    
    
    //SPI Clock Edge Select bit
    SSPSTATbits.CKE = 1;
    TRISC3 = 0; // Pin de salida CLK
}

void spiWrite(char dat){
    // Escribir datos en bus de datos
    SSPBUF = dat;
}

char spiRead(){
    // Leer los datos recibidos
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
    return(SSPBUF); // read the received data from the buffer
    __delay_ms(1);
}