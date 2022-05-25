/*
 * File:   eeprom.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:02
 */

#include "../eLock.X/init.h"
#include "../eLock.X/eeprom.h"
#include "../eLock.X/lcd.h"

void main(void) {
    LCD_Init();
    while (1) {
        char pin[] = "123";
        savePin(pin);
        char pin2[4] = ""; 
        readPin(pin2);
        if (strcmp(pin, pin2) == 0) {
            LCD_String_xy(1, 0, "W/R is good");
        } else {
            LCD_String_xy(1, 0, "W/R is not good");
        }
    }
    
}
