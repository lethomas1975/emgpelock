/*
 * File:   elock.c
 * Author: thomas
 *
 * Created on 19 May 2022, 21:50
 */

#include "init.h"
#include "common.h"
#include "elock.h"
#include "keypad.h"
#include "lcd.h"
#include "eeprom.h"
#include "buzzer.h"
#include "7seg.h"
#include "bluetooth.h"
#include "rgb.h"
#include "solenoid.h"

void resetPinHolders(void) {
    for (int i = 0; i < MAX_PIN_SIZE; i++) {
        currP[i] = 0;
        newP[i] = 0;
        confP[i] = 0;
    }
}

void clearCommandString(void) {
    clearString(commandBT, MAX_COMMAND_SIZE);
    index = 0;
}

void displayInstruction(void) {
    LCD_String_xy(1, 0, "eLock by Team C2");
    LCD_String_xy(2, 1, "Press any key");    
}

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

void toggleLock(void) {
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
    delayInMs(500);
    LCD_Clear();
}

void askPin(const char* message, char pin[MAX_PIN_SIZE]) {
    LCD_String_xy(1, 0, message);    
    int count = 0;
    while (count < 3) {
        char c = keyPressed();
        if (c != NULL) {
            LCD_Char('*');
            pin[count] = c;
            count++;
            delayInMs(500);
        }
    }
    pin[3] = 0;
}

char checkPin(char pin[MAX_PIN_SIZE]) {
    char savedPin[MAX_PIN_SIZE] = "";
    readPinFromEeprom(savedPin);
    return strcmp(savedPin, pin) == 0;
}

void handleConfirmPin(char confirmed, int *count) {
    if (!confirmed) {
        sendString(C2NOKPIN);
        switch (*count) {
            case 0:
                greenOn();
                break;
            case 1:
                blueOn();
                break;
            case 2:
                redOn();
                break;
        }
        setSevenSegment(*count);
        buzzOnAndOff(50);
        *count = *count + 1;
    } else {
        sendString(C2OKPIN);
        clearSevenSegment();
        rgbOff();
        *count = 0;
    }
}

void handleChangePin(void) {
    if (changePin(currP, newP, confP)) {
        sendString(C2OKCP);
        LCD_Clear();
        LCD_String_xy(1, 0, "Pin changed!");
        pinCount = 0;
        delayInMs(500);
    } else {
        sendString(C2NOKCP);
        LCD_Clear();
        LCD_String_xy(1, 0, "Pin unchanged!");
        setSevenSegment(pinCount);
        buzzOnAndOff(500);
        pinCount++;
        systemLocked();
    }
    LCD_Clear();
}

void askForChangePin(char currPin[MAX_PIN_SIZE], char newPin[MAX_PIN_SIZE], char confPin[MAX_PIN_SIZE]) {
    LCD_Clear();
    askPin("Curr PIN: ", currPin);
    LCD_Clear();
    askPin("New PIN: ", newPin);
    LCD_Clear();
    askPin("Conf PIN: ", confPin);
}

char confirmPin(const char pin1[MAX_PIN_SIZE], const char pin2[MAX_PIN_SIZE]) {
    return strcmp(pin1, pin2) == 0;
}

void setupPin(void) {
    char sPin[MAX_PIN_SIZE] = ""; 
    readPinFromEeprom(sPin);
    if (sPin[0] == 0) {
        savePinToEeprom("000");
    }
}

char changePin(const char oldP[MAX_PIN_SIZE], const char newP[MAX_PIN_SIZE], const char conP[MAX_PIN_SIZE]) {
    char sPin[MAX_PIN_SIZE] = "";
    readPinFromEeprom(sPin);
    char confirmed = confirmPin(sPin, oldP);
    char confirmed2 = confirmPin(sPin, newP);
    char confirmed3 = confirmPin(newP, conP);
    if (confirmed && !confirmed2 && confirmed3) {
        savePinToEeprom(newP);
        return 1;
    }
    return 0;
}

void parsePin(void) {
    if (strlen(commandBT) != MAX_COMMAND_SIZE) {
        resetPinHolders();
        char encrypted = readEncryptFromEeprom() == '1';
        for (int i = 0; i < MAX_PIN_SIZE - 1; i++) {
            currP[i] = commandBT[6 + i];
        }
        currP[3] = 0;
        if (encrypted) {
            decrypt(currP);
        }
    }
}

void parseChangePin(char cmd[MAX_COMMAND_SIZE]) {
    resetPinHolders();
    char encrypted = readEncryptFromEeprom() == '1';
    for (int i = 0; i < MAX_PIN_SIZE - 1; i++) {
        currP[i] = cmd[5 + i];
        newP[i] = cmd[9 + i];
        confP[i] = cmd[13 + i];
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

void systemLocked(void) {
    if (pinCount == 3) {
        sendString(C2SYSTLOCK);
        delayInMs(50);
        BTResetOut = 0;
        redOn();
        setSevenSegment(2);
        buzzOnAndOff(5000);
        rgbOff();
        clearSevenSegment();
        BTResetOut = 1;
        pinCount = 0;
    }
}

void disableInterrupt(void) {
    GIE = 0;
}

void enableInterrupt(void) {
    LATB = 0xf0;
    RBIF = 0;
    RCIF = 0;
    GIE = 1;
    RBIF = 0;
    RCIF = 0;
}

void setupEncrypt(void) {
    char sEncrypt = readEncryptFromEeprom();
    if (sEncrypt == NULL) {
        saveEncryptToEeprom('0');
    }
}

void toggleEncrypt(void) {
    char sEncrypt = readEncryptFromEeprom() == '1';
    if (!sEncrypt) {
        saveEncryptToEeprom('1');
    } else {
        saveEncryptToEeprom('0');        
    }
    sendEncryptStatus();
    LCD_Clear();
    LCD_String_xy(1, 0, readEncryptFromEeprom() == 0 ? "Encrypt disabled" : "Encrypt enabled");
    delayInMs(500);
    LCD_Clear();
}

void sendEncryptStatus(void) {
    char encrp = readEncryptFromEeprom();
    char enc_status[8] = "";
    strcpy(enc_status, C2OKE);
    enc_status[6] = encrp;
    sendString(enc_status);
}

void sendAppStatus(void) {
    char encrp = readEncryptFromEeprom();
    char app_status[10] = "";
    strcpy(app_status, C2OKE);
    app_status[6] = encrp;
    app_status[7] = '+';
    app_status[8] = isLocked() ? 'L' : 'U';
    app_status[9] = 0;
    sendString(app_status);    
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

void handleBTCommand(void) {
    if (commandBT[0] == 0) {
        return;
    }
    int cmd = parseCommand(commandBT);
    char comd = NULL;
    switch (cmd) {
        case 1:
            sendAppStatus();
            clearCommandString();
            break;
        case 2:
            clearCommandString();
            break;
        case 3:
            comd = commandBT[3];
            int len = strlen(commandBT);
            if (comd == '3') {
                if (len >= 16) {
                    parseChangePin(commandBT);
                    command = '7';
                    clearCommandString();
                }
            } else if (comd == '4') {
                command = '8';
                clearCommandString();
            } else if (comd >= '1' && comd < '5') {
                command = comd + 3;
                clearCommandString();
            } else if (comd == '0') {
                sendAppStatus();
                command = '0';
                clearCommandString();
            }
            break;
        case 4:
            parsePin();
            command = '2';
            clearCommandString();
            break;
        default:
            command = '0';
            break;
    }
}

