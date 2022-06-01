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
#include "common.h"
#include "buzzer.h"
#include "7seg.h"
#include "init.h"
#include "bluetooth.h"
#include "rgb.h"
#include "solenoid.h"

void askPin(const char* message, char pin[4]) {
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

char checkPin(const char pin[4]) {
    char savedPin[4] = "";
    readPinFromEeprom(savedPin);
    return strcmp(savedPin, pin) == 0;
}

char login(void) {
    int count = 0;
    while (count < 3) {
        LCD_Clear();
        char pin[4] = "";
        askPin("PIN: ", pin);
        char confirmed = checkPin(pin);
        handleConfirmPin(confirmed, &count);
    }
    if (count == 3) {
        systemLocked();
        return 0;
    }
    return 1;
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
        *count = 4;
    }
}

void systemLocked(void) {
    sendString(C2SYSTLOCK);
    delayInMs(50);
    BTResetOut = 0;
    redOn();
    setSevenSegment(2);
    buzzOnAndOff(5000);
    rgbOff();
    clearSevenSegment();
    BTResetOut = 1;
}

void askForChangePin(char currPin[4], char newPin[4], char confPin[4]) {
    LCD_Clear();
    askPin("Curr PIN: ", currPin);
    LCD_Clear();
    askPin("New PIN: ", newPin);
    LCD_Clear();
    askPin("Conf PIN: ", confPin);
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

char confirmPin(const char pin1[4], const char pin2[4]) {
    return strcmp(pin1, pin2) == 0;
}

void setupPin(void) {
    char sPin[4] = ""; 
    readPinFromEeprom(sPin);
    if (sPin[0] == 0) {
        savePinToEeprom("000");
    }
}

char changePin(const char oldP[4], const char newP[4], const char conP[4]) {
    char sPin[4] = "";
    readPinFromEeprom(sPin);
    char confirmed = confirmPin(sPin, oldP);
    char confirmed2 = confirmPin(sPin, newP);
    if (confirmed && !confirmed2) {
        confirmed = confirmPin(newP, conP);
        if (confirmed) {
            savePinToEeprom(newP);
            return 1;
        }
    }
    return 0;
}

void setupEncrypt(void) {
    char sEncrypt = readEncryptFromEeprom();
    if (sEncrypt == NULL) {
        saveEncryptToEeprom('0');
    }
}

void toggleEncrypt(void) {
    char sEncrypt = readEncryptFromEeprom();
    if (!sEncrypt) {
        saveEncryptToEeprom('1');
    } else {
        saveEncryptToEeprom('0');        
    }
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
