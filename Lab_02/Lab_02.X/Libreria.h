/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Configuraciones de modulos
void Config_Oscilador(void);
char Config_TMR0(void);
void Config_ADC(void);
void Config_USART(void);
void Config_Puertos(void);

// Funciones de programa
char Valor_ADC(char canal);
void Transmisor_USART(char valor);
void tabla_USART(int numero);
void USART(unsigned char canal_10,unsigned char canal_12);
void Texto_USART(char texto[]);
void LCD(unsigned char canal_10,unsigned char canal_12);
#endif	/* XC_HEADER_TEMPLATE_H */

