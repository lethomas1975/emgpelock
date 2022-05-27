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
char oldP[4] = "";
char newP[4] = "";
char conP[4] = "";

char commandBT[17] = "";
char index = 0;
char data = 0;
char stop = 0;

int parseCommand(const char * recv);
void handleCommand(void);
void clearCommandString(void);

void main(void) {
    LCD_Init();
    OSCCON = 0x72;

    setupPin();
    setupEncrypt();
    LCD_Clear();
    clearCommandString();
    
    while (1) {
        char e = 0;
        handleCommand();
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
                LCD_String_xy(1, 0, "1-Lock 2-Unlock ");
                LCD_String_xy(2, 0, "3- New PIN 4-Rst");
                break;
            case '3':
                GIE = 0;
                lock();
                sendString(C2OKSL);
                LCD_Clear();
                LCD_String_xy(1, 0, "Locked!");
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
                sendString(C2OKSU);
                LCD_Clear();
                LCD_String_xy(1, 0, "Unlocked!");
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
                sendString(C2OKCP);
                LCD_Clear();
                LCD_String_xy(1, 0, "Pin changed!");
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;
            /*case '7':
                GIE = 0;
                toggleEncrypt();
                sendEncryptStatus();
                e = readEncrypt();
                LCD_Clear();
                LCD_String_xy(1, 0, e == 0 ? "Encrypt disabled" : "Encrypt enabled");
                delayInMs(1000);
                LCD_Clear();
                LATB = 0xf0;
                GIE = 1;
                RBIF = 0;
                RCIF = 0;
                command = '0';
                break;*/
            case '6':
                GIE = 0;
                resetBT();
                LCD_Clear();
                sendString(C2OKRBT);
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
                if (command < '3' || command > '6') {
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
        commandBT[index] = c;
        index = (index + 1) % 16;
    }
    RCIF = 0;
    RCIP = 1;
    RCIF = 0;
}

int parseCommand(const char * recv) {
    if (isOkConn(recv)) {
        return 1;
    }
    if (isOkLost(recv)) {
        return 2;
    }
    if (isC2Command(recv)) {
        return 3;
    }
    if (isC2Pin(recv)) {
        return 4;
    }
    return 0;
}

void handleCommand(void) {
    if (commandBT[0] == 0) {
        return;
    }
    //LCD_String(commandBT);
    int cmd = parseCommand(commandBT);
    switch (cmd) {
        case 1:
            sendAppStatus();
            clearCommandString();
            break;
        case 2:
            clearCommandString();
            break;
        case 3:
            command = commandBT[3];
            if (command == '3') {
                for (int i = 0; i < 3; i++) {
                    oldP[i] = commandBT[5 + i];
                    newP[i] = commandBT[9 + i];
                    conP[i] = commandBT[13 + i];
                }
                oldP[3] = 0;
                newP[3] = 0;
                conP[3] = 0;
                changePinBT(oldP, newP, conP);
            } else if (command >= '1' && command <= '5') {
                command = command + 2;
            } else if (command == '0') {
                sendAppStatus();
            } else {
                sendString("C2NOK");
            }
            clearCommandString();
            break;
        case 4:
            for (int i = 0; i < 3; i++) {
                oldP[i] = commandBT[6 + i];
            }
            oldP[3] = 0;
            if (checkPin(oldP)) {
                sendString(C2OKPIN);
            } else {
                sendString(C2NOKPIN);
            }
            clearCommandString();
            break;
    }
}

void clearCommandString(void) {
    for (int i = 0; i < 17; i++) {
        commandBT[i] = 0;
    }
    index = 0;
}