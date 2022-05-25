/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:42
 */

#include "init.h"
#include "lcd.h"
#include "keypad.h"
#include "elock.h"
#include "7seg.h"
#include "buzzer.h"
#include "eeprom.h"
#include "solenoid.h"
#include "common.h"
#include "bluetooth.h"

int error = 0;
char command = '0';

void main(void) {
    LCD_Init();
    OSCCON = 0x72;

    setupPin();
    setupEncrypt();
    LCD_Clear();
    while (1) {
        switch (command) {
            case '0':
                LCD_String_xy(1, 0, "eLock by Team C2");
                LCD_String_xy(2, 1, "Press any key");
                break;
            case '1':
                RBIE = 0;
                char confirmed = askForPin();
                setSevenSegment(3);
                LCD_Clear();
                RBIF = 0;
                LATB = 0xf0;
                RBIE = 1;
                RBIF = 0;
                if (confirmed) {
                    command = '2';
                } else {
                    command = '0';                    
                }
                break;
            case '2':
                LCD_String_xy(1, 0, "1-L 2-U 3-Ch PIN");
                LCD_String_xy(2, 0, "4-Enc 5-Rst BT");
                break;
            case '3':
                GIE = 0;
                lock();
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;
            case '4':
                GIE = 0;
                unlock();
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;
            case '5':
                GIE = 0;
                changePin();
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;
            case '6':
                GIE = 0;
                toggleEncrypt();
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;
            case '7':
                GIE = 0;
                resetBT();
                LCD_Clear();
            default:
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';     
                break;
        }
    }
}

void interrupt low_priority keypad(void) {
    if (RBIF == 1) {
        RBIE = 0;
        LATB = 0xff;
        PORTB;
        if (command == '0') {
            LCD_Clear();
            command = '1';
        } else if (command == '2') {
            char c = keyPressed();
            if (c != NULL && c != '#' && c != '*') {
                command = c + 2;
                if (command < '3' || command > '7') {
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Wrong choice!");
                    buzz();
                    delayInMs(1000);
                    buzzOff();
                    LCD_Clear();
                    command = '2';
                }
            }
        }
        buzzOff();
    }
    RBIF = 0;
    LATB = 0xf0;
    RBIE = 1;
    RBIF = 0;        
}

void interrupt usart(void) {
    if (RCIF == 1) {
        RCIP = 0;
        char c = receiveChar();
        command = c + 1;
    }
    RCIF = 0;
    RCIP = 1;
    RCIF = 0;
}