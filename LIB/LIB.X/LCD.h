/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#ifndef RS
#define RS PORTEbits.RE0
#define EN PORTEbits.RE1

#define D0 PORTAbits.RA0
#define D1 PORTAbits.RA1
#define D2 PORTAbits.RA2
#define D3 PORTAbits.RA3
#define D4 PORTAbits.RA4
#define D5 PORTAbits.RA5
#define D6 PORTAbits.RA6
#define D7 PORTAbits.RA7
#endif
void LCD_Init_8bits(void);
void PORT_LCD(char v);
void CMD_LCD(char v);
void Lcd_Set_Cursor(char a, char b);
void Clear_LCD(void);
void Char_LCD(char a);
void Write_LCD(char *a);
void Print_Num(char valor);
void Print_Cont(char valor);
void tabla_num(int numero);

#endif	/* XC_HEADER_TEMPLATE_H */