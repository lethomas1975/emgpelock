/*
 * File:   lcd.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:23
 */


#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/lcd.h"

/**
 * LCD_Init()
 * LCD init
 * 
 * Author: provided by electronicwings
 */
void LCD_Init() {
    LCD_Command(0x38);     /* uses 2 line and initialize 5*8 matrix of LCD */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x02);     /* display on cursor off */
    LCD_Command(0x80);
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

/**
 * LCD_Clear()
 * clear the LCD display
 * 
 * Author: provided by electronicwings
 */
void LCD_Clear() {
    LCD_Command(0x01); /* clear display screen */
}

/**
 * LCD_Command()
 * execute the command for the LCD
 * 
 * Parameters:
 *  cmd: command to execute
 * 
 * Author: provided by electronicwings
 */
void LCD_Command(char cmd) {
	LCDOut = cmd;            /* Send data to PORT as a command for LCD */   
	LCDA0Out = 0;            /* Command Register is selected */
	LCDA1Out = 1;            /* High-to-Low pulse on Enable pin to latch data */ 
	NOP();
	LCDA1Out = 0;
	delayInMs(5);
}

/**
 * LCD_Char()
 * display a character on the LCD at the current position
 * 
 * Parameters:
 *  data: character to display
 * 
 * Author: provided by electronicwings
 */
void LCD_Char(char data)
{
	LCDOut = data;            /* Send data to LCD */  
	LCDA0Out = 1;            /* Data Register is selected */
	LCDA1Out = 1;            /* High-to-Low pulse on Enable pin to latch data */   
	NOP();
	LCDA1Out = 0;
	delayInMs(5);
}


/**
 * LCD_String()
 * display a string on the LCD at the current position
 * 
 * Parameters:
 *  msg: string to display
 * 
 * Author: provided by electronicwings
 */
void LCD_String(const char *msg) {
	while( *msg != 0) {		
        LCD_Char(*msg);
        msg++;	
    }
}

/**
 * LCD_String_xy()
 * display a string on the LCD
 * 
 * Parameters:
 *  row: which row to display the first character
 *  pos: position on the row to display the first charater
 *  msg: string to display
 * 
 * Author: provided by electronicwings
 */
void LCD_String_xy(char row, char pos, const char *msg) {
    char location = 0;
    if(row <= 1) {
        location = (0x80) | (pos & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    } else {
        location = (0xC0) | (pos & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);
}