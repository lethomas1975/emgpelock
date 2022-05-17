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
    init();
    LCD_Init();
    LCD_String_xy(1, 0, "Hello: ");
    LCD_Command(0xC0);
    LEDPin = 1;
    delayInMs(50);
    int i = 1;
    while(1) {
        LEDPin = 0;
        char c = keyPressed();
        if (c != NULL) {
            LCD_Char(c);
        }
        /*if (c != NULL) {
            if (i >= 10) {
                switch (i) {
                    case 10:
                        if (c == '*') {
                            LEDPin = 1;
                            i++;
                        }
                        break;
                    case 11:
                        if (c == 0) {
                            LEDPin = 1;
                            i++;
                        }
                        break;
                    case 12:
                        if (c == '#') {
                            LEDPin = 1;
                            i++;
                        }
                        break;
                }
            } else if (i < 10 && c == i) {
                LEDPin = 1;
                i++;
            } else if (i > 12) {
                i = 1;
            }
            delayInMs(10);
        }*/
    }
}
