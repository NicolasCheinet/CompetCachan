// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __LCD_H_
#define	__LCD_H_

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>
#define _XTAL_FREQ 64000000

typedef enum{
    FIRST_LINE,
    SECOND_LINE,     
}LCD_POSITION;

typedef enum{
    COMMAND,
    DATA,
}LCD_REGISTER_TYPE;

extern void LCD_Init();
extern void LCD_Clear();
extern void LCD_Write(uint8_t byte, LCD_REGISTER_TYPE type);
extern void LCD_Goto(char ligne, char colonne);
extern void LCD_Print(char *t);

#endif