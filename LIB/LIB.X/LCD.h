/* 
 * File: LCD.h
 * Se utiliz? y se adaptaron las librer?as de Ligo George 
 * de la p?gina www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef LCD_H
#define    LCD_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef D0
#define D0 RA0
#endif

#ifndef D1
#define D1 RA1
#endif

#ifndef D2
#define D2 RA2
#endif

#ifndef D3
#define D3 RA3
#endif

#ifndef D4
#define D4 RA4
#endif

#ifndef D5
#define D5 RA5
#endif

#ifndef D6
#define D6 RA6
#endif

#ifndef D7
#define D7 RA7
#endif

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>
//LCD Functions Developed by electroSome

unsigned char a = 0;
void Lcd_Port(unsigned char a);

void Lcd_Cmd(unsigned char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(unsigned char a,unsigned char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);

#endif    /* LCD_H */