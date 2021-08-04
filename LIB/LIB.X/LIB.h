/* 
 * File:    
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LIBR_H
#define	XC_LIBR_H

#include <xc.h> // include processor files - each processor file is guarded.  
// Configuraciones de modulos
void Config_Oscilador(void);
void Config_TMR0(void);
void Config_ADC(void);
void Config_USART(void);
void Config_Puertos(void);

// Funciones de programa
char Valor_ADC(char canal);
void tabla_USART(int numero);
void Texto_USART(char texto[]);

// Funciones especificas del programa
void SPI(volatile char *v1,volatile char *v2);
void USART_Num(char valor);
void texto_Programa(char v1, char v2);
#endif	/* XC_HEADER_TEMPLATE_H */

