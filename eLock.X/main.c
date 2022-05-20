/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:42
 */

#ifndef INTEGRATED
#define INTEGRATED
#endif

#include "init.h"
#include "lcd.h"
#include "keypad.h"
#include "elock.h"
#include "7seg.h"
#include "buzzer.h"

int error = 0;

void main(void) {
    LCD_Init();
    while (1);
}

void interrupt low_priority keypad(void) {
    if (RBIF == 1) {
        RBIE = 0;
        LCD_Clear();
        askForPin();
        char* pin = enterPin();
        char checked = checkPin(pin);
        if (!checked) {
            buzz();
            error++;
            setSevenSegment(error);
        }
        RBIF = 0;
    }
    buzzOff();
}

void interrupt usart(void) {
    if (RCIF == 1) {
        RCIP = 0;
        char c = RCREG;
        LCD_Char(c);
        RCIF = 0;
    }
}