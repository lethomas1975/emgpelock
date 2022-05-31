/*
 * File:   lcd.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:23
 */


#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/lcd.h"

void LCD_Init() {
    init();
    
    delayInMs(20);         /* 15ms,16x2 LCD Power on delay */
    LCDOut = 0;
    LCDTrisOut = 0;
    LCDA0Out = 0;
    LCDA0TrisOut = 0;
    LCDA1Out = 0;
    LCDA1TrisOut = 0;
    LCD_Command(0x38);     /* uses 2 line and initialize 5*8 matrix of LCD */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x02);     /* display on cursor off */
    LCD_Command(0x80);
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_Clear() {
    LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd) {
	LCDOut = cmd;            /* Send data to PORT as a command for LCD */   
	LCDA0Out = 0;            /* Command Register is selected */
	LCDA1Out = 1;            /* High-to-Low pulse on Enable pin to latch data */ 
	NOP();
	LCDA1Out = 0;
	delayInMs(5);
}

void LCD_Char(char dat)
{
	LCDOut = dat;            /* Send data to LCD */  
	LCDA0Out = 1;            /* Data Register is selected */
	LCDA1Out = 1;            /* High-to-Low pulse on Enable pin to latch data */   
	NOP();
	LCDA1Out = 0;
	delayInMs(5);
}


void LCD_String(const char *msg) {
	while( *msg != 0) {		
        LCD_Char(*msg);
        msg++;	
    }
}

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