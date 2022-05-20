/*
 * File:   elock.c
 * Author: thomas
 *
 * Created on 19 May 2022, 21:50
 */


#include "elock.h"
#include "keypad.h"
#include "lcd.h"
#include "eeprom.h"

char checkPin(const char * pin) {
    
}

void askForPin(void) {
    LCD_String_xy(1, 0, "Enter PIN: ");
    LCD_Command(0xC0);
}

char* enterPin() {
    char *pin = 0;
    int count = 0;
    while (count < 3) {
        char c = keyPressed();
        if (c != NULL) {
            LCD_Char(c);
            *pin = c;
            *pin++;
            count++;
        }
    }
    *pin = 0;
    return pin;
}

char checkPin(const char * pin) {
    char* savedPin = EEPROM_Read(PIN_START_ADDRESS);
    return strcmp(savedPin, pin) == 0;
}

