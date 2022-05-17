/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:45
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/bluetooth.h"
#include "../eLock.X/lcd.h"
#include <plib/usart.h>

char out = '\0';

void main(void) {
    init();
    LCD_Init();
    LCD_String_xy(1,0,"");
    LCD_String_xy(1,0,"Receive: ");
    LCD_Command(0xC0);
    char* welcome = "Hello Thomas\0";
    char* command = "\0";
    while (1) {
        char c = receiveChar();
        LCD_Char(c);
        sendCharacter(c);
    }
    
}
