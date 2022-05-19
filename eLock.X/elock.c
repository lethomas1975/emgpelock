/*
 * File:   elock.c
 * Author: thomas
 *
 * Created on 19 May 2022, 21:50
 */


#include "elock.h"
#include "keypad.h"
#include "lcd.h"

char checkPin(const char * pin) {
    
}

char* askForPin(void) {
    LCD_Clear();
    LCD_String_xy(1, 0, "Enter PIN: ");
    LCD_Command(0xC0);
    return enterPin();
}
