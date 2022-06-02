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

char command = '0';
char currP[4] = "";
char newP[4] = "";
char confP[4] = "";

char commandBT[17] = "";
char index = 0;
int pinCount = 0;

int parseCommand(const char * recv);
void parseChangePin(void);
void parsePin(void);
void handleCommand(void);
void decrypt(char toDecrypt[4]);

void displayMenu(void);
void resetPinHolders(void);
void clearCommandString(void);

void displayMenu(void) {
    char locked = isLocked();
    char encrypted = readEncryptFromEeprom() == '1';
    
    char message[16] = "1-";
    
    if (locked) {
        strcat(message, "ULck ");
    } else {
        strcat(message, "Lck ");
    }
    strcat(message, "2-");
    if (encrypted) {
        strcat(message, "Decrypt");        
    } else {
        strcat(message, "Encrypt");
    }
    
    LCD_String_xy(1, 0, message);
    LCD_String_xy(2, 0, "3-Chg PIN 4-RST");
}

void resetPinHolders(void) {
    disableInterrupt();
    for (int i = 0; i < 4; i++) {
        currP[i] = 0;
        newP[i] = 0;
        confP[i] = 0;
    }
    enableInterrupt();
}

void clearCommandString(void) {
    disableInterrupt();
    for (int i = 0; i < 17; i++) {
        commandBT[i] = 0;
    }
    index = 0;
    enableInterrupt();
}

void main(void) {
    LCD_Init();
    OSCCON = 0x72;

    savePinToEeprom("000");
    setupPin();
    setupEncrypt();
    LCD_Clear();
    resetPinHolders();
    clearCommandString();
    
    while (1) {
        handleCommand();
        switch (command) {
            case '0':
                LCD_String_xy(1, 0, "eLock by Team C2");
                LCD_String_xy(2, 1, "Press any key");
                break;
            case '1':
                disableInterrupt();
                clearCommandString();
                char confirmed = login();
                LCD_Clear();
                enableInterrupt();
                if (confirmed) {
                    command = '2';
                } else {
                    command = '0';                    
                }
                break;
            case '2':
                displayMenu();
                break;
            case '3':
                disableInterrupt();
                clearCommandString();
                if (isLocked()) {
                    unlock();
                    sendString(C2OKSU);
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Unlocked!");
                } else {
                    lock();
                    sendString(C2OKSL);
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Locked!");
                }
                delayInMs(1000);
                LCD_Clear();
                enableInterrupt();
                command = '0';
                break;
            case '4':
                disableInterrupt();
                clearCommandString();
                toggleEncrypt();
                sendEncryptStatus();
                LCD_Clear();
                LCD_String_xy(1, 0, readEncryptFromEeprom() == 0 ? "Encrypt disabled" : "Encrypt enabled");
                delayInMs(1000);
                LCD_Clear();
                enableInterrupt();
                command = '0';
                break;
            case '5':
                disableInterrupt();
                resetPinHolders();
                delayInMs(500);
                askForChangePin(currP, newP, confP);
            case '6':
                disableInterrupt();
                clearCommandString();
                if (changePin(currP, newP, confP)) {
                    sendString(C2OKCP);
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Pin changed!");
                    delayInMs(1000);
                    LCD_Clear();
                } else {
                    sendString(C2NOKCP);
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Pin unchanged!");
                    buzzOnAndOff(1000);
                }
                LCD_Clear();
                enableInterrupt();
                command = '0';
                break;
            case '7':
                disableInterrupt();
                clearCommandString();
                sendString(C2OKRBT);
                delayInMs(50);
                systemLocked();
                LCD_Clear();
                enableInterrupt();
                command = '0';
                break;
            default:
                enableInterrupt();
                command = '0';     
                break;
        }
    }
}

void interrupt low_priority keypad(void) {
    if (RBIF == 1) {
        disableInterrupt();
        RBIE = 0;
        LATB = 0xff;
        PORTB;
        if (command == '0') {
            LCD_Clear();
            command = '1';
        } else if (command == '2') {
            char c = keyPressed();
            if (c != NULL && c != '#' && c != '*') {
                if (c == '4') {
                    command = '7';
                } else {
                    command = c + 2;
                }
                if (command < '3' || command > '7') {
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Wrong choice!");
                    buzzOnAndOff(1000);
                    buzzOff();
                    LCD_Clear();
                    command = '2';
                }
            }
        }
        buzzOff();
        LATB = 0xf0;
        RBIE = 1;
        enableInterrupt();
    }
    RBIF = 0;        
}

void interrupt usart(void) {
    if (RCIF == 1) {
        disableInterrupt();
        RCIP = 0;
        char c = receiveChar();
        commandBT[index] = c;
        index = (index + 1) % 16;
        RCIF = 0;
        enableInterrupt();
        RCIP = 1;
    }
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
    int cmd = parseCommand(commandBT);
    switch (cmd) {
        case 1:
            sendAppStatus();
            break;
        case 2:
            break;
        case 3:
            command = commandBT[3];
            if (command == '3') {
                parseChangePin();
                command = '6';
            } else if (command == '4') {
                command = '7';
            } else if (command >= '1' && command <= '5') {
                command = command + 2;
            } else if (command == '0') {
                sendAppStatus();
            } else {
                sendString(C2NOK);
            }
            break;
        case 4:
            parsePin();
            handleConfirmPin(checkPin(currP), &pinCount);
            if (pinCount >= 3) {
                systemLocked();
                pinCount = 0;
            }
            break;
        default:
            command = '0';
            break;
    }
    clearCommandString();
}

void parsePin(void) {
    resetPinHolders();
    char encrypted = readEncryptFromEeprom() == '1';
    for (int i = 0; i < 3; i++) {
        currP[i] = commandBT[6 + i];
    }
    currP[3] = 0;
    if (encrypted) {
        decrypt(currP);
    }    
}

void parseChangePin(void) {
    resetPinHolders();
    char encrypted = readEncryptFromEeprom() == '1';
    for (int i = 0; i < 3; i++) {
        currP[i] = commandBT[5 + i];
        newP[i] = commandBT[9 + i];
        confP[i] = commandBT[13 + i];
    }
    currP[3] = 0;
    newP[3] = 0;
    confP[3] = 0;
    if (encrypted) {
        decrypt(currP);
        decrypt(newP);
        decrypt(confP);
    }
}

/* simple decryption: only the pin is encrypted
 * a -> 0, b -> 1, ... j -> 9 
 */
void decrypt(char toDecrypt[4]) {
    LCD_Clear();
    LCD_String_xy(1, 0, "Decrypting: ");
    LCD_String(toDecrypt);
    char tmp[4] = "";
    for (int i = 0; i < 3; i++) {
        tmp[i] = toDecrypt[i] - 1 - i;
    }
    tmp[3] = 0;
    strcpy(toDecrypt, tmp);
}