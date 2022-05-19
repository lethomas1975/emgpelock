/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:36
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/keypad.h"
#include "../eLock.X/lcd.h"

void main(void) {
    LCD_Init();
    LCD_String_xy(1, 0, "Hello: ");
    while(1) {
        if (OSCCONbits.OSTS == 1) {
            LCD_Char('1');
        }
        if (OSCCONbits.IOFS == 0) {
            LCD_Char('2');
        }
        LEDPin = 1;
        delayInMs(100);
        LEDPin = 0;
        char c = keyPressed();
        if (c != NULL) {
            LCD_Char(c);
        }
    }
}
