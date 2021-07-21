/*
 * File:   Libreria.c
 * Author: fredy
 *
 * Created on July 20, 2021, 2:29 PM
 */


#include <xc.h>
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
char ADC_Valor(char canal){
    ADCON0bits.CHS = canal;
    return ADRESH;
}

