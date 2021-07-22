/*
 * File:   Libreria.c
 * Author: fredy
 *
 * Created on July 20, 2021, 2:29 PM
 */


#include <xc.h>
char Tabla_Display (char numero){
    char tabla [16]= {
        0b1000000, // Cero A //0b0111111, // Cero
        0b1111001, // Uno A  //0b0000110, // Uno
        0b0100100, // Dos A  //0b1011011, // Dos
        0b0110000, // Tres A //0b1001111, // Tres
        0b0011001, // Cuatro A //0b1100110, // Cuatro
        0b0010010, // Cinco A //0b1101101, // Cinco
        0b0000010, // Seis A //0b1111101, // Seis 
        0b1111000, // Siete A //0b0000111, // Siete
        0b0000000, // Ocho A//0b1111111, // Ocho
        0b0011000, // Nueve A //0b1100111, // Nueve
        0b0001000, // A A//0b1110111, // A
        0b0000011, // B A//0b1111100, // B
        0b1000110, // C A //0b0111001, // C
        0b0100001, // D A//0b1011110, // D
        0b0000110, // E A//0b1111001, // E
        0b0001110, // F A//0b1110001, // F
    }; 
    return tabla[numero];
}

char ADC_Valor(char canal){
    ADCON0bits.CHS = canal;
    return ADRESH;
}

