/* 
 * File: Libreria SPI 
 * Author: Fredy Godoy 19260
 * Comments: 
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_SPI_H
#define	XC_SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
void Init_Master(void);
void Init_Slave(void);
void spiWrite(char dat);
char spiRead(); 

#endif	/* XC_HEADER_TEMPLATE_H */

