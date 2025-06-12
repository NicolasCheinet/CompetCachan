#include "LCD.h"


#include <xc.h>
#include <stdint.h>

////////////////////////////////////////////////////////
// Don't touch these values
#define _4BIT 0
#define _8BIT 1
#define _ON 1
#define _OFF 0
#define _ONE_LINE 0
#define _TWO_LINES 1
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Place your LCD customizations here
#define MODE _4BIT // Valid options are _4BIT or _8BIT
#define CURSOR _OFF // Valid options are ON or OFF
#define BLINK _OFF // Valid option are ON or OFF
#define NUMLINES _TWO_LINES // Valid options are ONE_LINE or TWO_LINES
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//Replace the following pin definitions with your hardware
#define EN LATBbits.LB4
//#define RW LATAbits.LA3
#define RS LATBbits.LB5
#define D4 LATBbits.LB3
#define D5 LATBbits.LB2
#define D6 LATBbits.LB1
#define D7 LATDbits.LD7
//#define RE LATDbits.LD5
/////////////////////////////////////////////////////////


static void lcdNibble(uint8_t nibble);
static void lcdByte(uint8_t byte);

/**
 *  Initialize LCD according to page 45, FIGURE 24 on the Hitachi datasheet
 */
extern void LCD_Init(){
    RS = 0;
    EN = 0;
    
    __delay_ms(40); // wait 10ms for lcd internal initialization
    
#if(MODE == _4BIT)
    // First send the HIGH nibble of the Function set
    lcdNibble(0b0011); // Function set - Interface defaults to 8-bit mode
    __delay_ms(5);
    lcdNibble(0b0011);// Function set - Interface defaults to 8-bit mode
    __delay_ms(5);
    lcdNibble(0b0011);// Function set - Interface defaults to 8-bit mode
    __delay_ms(5);
    lcdNibble(0b0010); // Now set the interface to 4-bit mode
    __delay_ms(5);
#elif(MODE == _8BIT)
    LCD_Write(0b00110000);
    __delay_ms(5);
    LCD_Write(0b00110000);
    __delay_ms(5);
    LCD_Write(0b00110000);
    __delay_ms(5);s
#else
#error Bit Mode not defined
#endif
    //volatile uint8_t test = ((0b1100) | (CURSOR << 1) | (BLINK << 0));
    LCD_Write(((0b0010 << 4) | (NUMLINES << 3)), COMMAND); // 0x28 = 4-bit, 2-line, 5x8 font size
    __delay_ms(5); 
    LCD_Write(((0b1100) | (CURSOR << 1) | (BLINK << 0)), COMMAND); // 0x0C = Turn ON display, no cursor, no blinking
    __delay_ms(8);
    LCD_Clear(); // Clear the screen
    __delay_ms(5);
    LCD_Write(0x06, COMMAND); // Move from left-to-right, no shifting
    __delay_ms(5);
    LCD_Clear();
    
    RS = 1;
    
}

/**
 * Sets the cursor to the beginning of first or second line
 * 
 * @param pos
 */
extern void LCD_Goto(char ligne, char colonne)
 {
    RS = 0;
    
     if(ligne == 0)
        LCD_Write(0x80 + colonne, COMMAND);
     else
       LCD_Write(0x80 + 0x40 + colonne, COMMAND);
 }

/**
 * Clears the LCD
 */
extern void LCD_Clear(){
    LCD_Write(0x01, COMMAND);
    __delay_ms(2);
}

/**
 * Write a byte to the LCD
 * @param byte 8-bit data
 * @param type RS pin depends on data type if command or not
 */
extern void LCD_Write(uint8_t byte, LCD_REGISTER_TYPE type){
    if(type == COMMAND) // check expected data type
        RS = 0; // sending special commands (see hd44780 datasheet)
    else
        RS = 1; // assume actual data
    
    __delay_us(100);
#if(MODE == _4BIT)
    lcdNibble(byte >> 4); // send higher 4-bits
    lcdNibble(byte & 0x0F); // send lower 4-bits
#else
    lcdByte(byte);
#endif
}

/**
 * Prints to the LCD without needing to use printf
 * @param t string of characters to send to lcd
 */
extern void LCD_Print(char *t){
    while(*t != '\0'){
        LCD_Write(*t++, DATA);
    }
}

/**
 * Used for 4-bit mode operation
 * @param nibble 4 bits of data to send to lcd
 */
static void lcdNibble(uint8_t nibble){
    D4 = (nibble & 0x01) ? 1 : 0;
    D5 = (nibble & 0x02) ? 1 : 0;
    D6 = (nibble & 0x04) ? 1 : 0;
    D7 = (nibble & 0x08) ? 1 : 0;
    
    // Now strobe
    EN = 1;
    __delay_us(100);
    EN = 0; 
    __delay_us(100);
}

/**
 * Used for 8-bit mode operation
 * @param byte
 */
static void lcdByte(uint8_t byte){
    /*D0 = (byte & 0x01) ? 1 : 0;
    D1 = (byte & 0x02) ? 1 : 0;
    D2 = (byte & 0x04) ? 1 : 0;
    D3 = (byte & 0x08) ? 1 : 0;*/
    D4 = (byte & 0x10) ? 1 : 0;
    D5 = (byte & 0x20) ? 1 : 0;
    D6 = (byte & 0x40) ? 1 : 0;
    D7 = (byte & 0x80) ? 1 : 0;
    
    // Now strobe
    EN = 1;
    __delay_us(100);
    EN = 0; 
    __delay_us(100);
}