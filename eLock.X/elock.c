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

char askForPin(void) {
    LCD_Clear();
    char confirmed = 0;
    int count = 0;
    char sPin[4] = "";
    readPin(sPin);
    while (count < 3) {
        LCD_String_xy(1, 0, "PIN: ");
        char pin[4] = "";
        enterPin(pin);
        confirmed = confirmPin(sPin, pin);
        if (!confirmed) {
            buzz();
            delayInMs(50);
            buzzOff();
            switch (count) {
                case 0:
                    RGBLedROut = 0;
                    RGBLedGOut = 1;
                    RGBLedBOut = 0;
                    break;
                case 1:
                    RGBLedROut = 0;
                    RGBLedGOut = 0;
                    RGBLedBOut = 1;
                    break;
                case 2:
                    RGBLedROut = 1;
                    RGBLedGOut = 0;
                    RGBLedBOut = 0;
                    break;
            }
            RGBLedROut = 1;
            setSevenSegment(count);
            LCD_Clear();
            count++;
        } else {
            RGBLedROut = 0;
            RGBLedGOut = 0;
            RGBLedBOut = 0;
            setSevenSegment(3);
            count = 4;
        }
    }
    if (count == 3) {
        BTResetOut = 0;
        delayInMs(5000);
        RGBLedROut = 0;
        RGBLedGOut = 0;
        RGBLedBOut = 0;
        BTResetOut = 1;
        return 0;
    }
    return 1;
}

void enterPin(char *pin) {
    int count = 0;
    while (count < 3) {
        char c = keyPressed();
        if (c != NULL) {
            LCD_Char('*');
            *pin = c;
            *pin++;
            count++;
            delayInMs(500);
        }
    }
    *pin = 0;
}

char checkPin(const char * pin) {
    char savedPin[4] = "";
    readPin(savedPin);
    return strcmp(savedPin, pin) == 0;
}

char confirmPin(const char* pin1, const char* pin2) {
    return strcmp(pin1, pin2) == 0;
}

void setupPin(void) {
    char sPin[4] = ""; 
    readPin(sPin);
    if (sPin[0] == 0) {
        savePin("000");
    }
}

void changePin(void) {
    char confirmed = askForPin();
    if (!confirmed) {
        return;
    }
    confirmed = 0;
    while (!confirmed) {
        LCD_String_xy(1, 0, "New PIN: ");
        char pin[4] = "";
        enterPin(pin);
        LCD_String_xy(1, 0, "Confirm PIN: ");
        char confPin[4] = "";
        enterPin(confPin);
        confirmed = confirmPin(pin, confPin);
        if (confirmed) {
           savePin(pin);
        } else {
            buzz();
            delayInMs(50);
            buzzOff();
        }
    }
}

char changePinBT(const char* oldP, const char* newP, const char* conP) {
    char sPin[4] = "";
    readPin(sPin);
    char confirmed = confirmPin(sPin, oldP);
    if (confirmed) {
        confirmed = confirmPin(newP, conP);
        if (confirmed) {
            savePin(newP);
            return 1;
        }
    }
    return 0;
}

void setupEncrypt(void) {
    char sEncrypt = readEncrypt();
    if (sEncrypt == NULL) {
        saveEncrypt('0');
    }
}

void toggleEncrypt(void) {
    char sEncrypt = readEncrypt();
    if (!sEncrypt) {
        saveEncrypt('1');
    } else {
        saveEncrypt('0');        
    }
}

void resetBT(void) {
    BTResetOut = 0;
    delayInMs(5000);
    BTResetOut = 1;
}

void sendEncryptStatus(void) {
    char encrp = readEncrypt();
    char enc_status[8] = "";
    strcpy(enc_status, C2OKE);
    enc_status[6] = encrp;
    sendString(enc_status);
}

void sendAppStatus(void) {
    char encrp = readEncrypt();
    char app_status[10] = "";
    strcpy(app_status, C2OKE);
    app_status[6] = encrp;
    app_status[7] = '+';
    app_status[8] = SolenoidOut == 1 ? 'L' : 'U';
    app_status[9] = 0;
    sendString(app_status);    
}
