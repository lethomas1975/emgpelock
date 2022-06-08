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

void main(void) {
    init();
    LCD_Init();
    OSCCON = 0x72;

    savePinToEeprom("000");
    setupPin();
    setupEncrypt();
    LCD_Clear();
    resetPinHolders();
    clearCommandString();
    
    while (1) {
        handleBTCommand();
        switch (command) {
            case '0':
                displayInstruction();
                break;
            case '1':
                disableInterrupt();
                resetPinHolders();
                LCD_Clear();
                askPin("PIN: ", currP);
            case '2':
                disableInterrupt();
                char confirmed = checkPin(currP);
                handleConfirmPin(confirmed, &pinCount);
                LCD_Clear();
                enableInterrupt();
                if (confirmed) {
                    if (command == '1') {
                        command = '3';
                    } else {
                        command = '0';
                    }
                } else {
                    if (pinCount == 3) {
                        command = '0';
                    }
                    systemLocked();
                    if (command == '2') {
                        command = '0';
                    }
                }
                break;
            case '3':
                displayMenu();
                break;
            case '4':
                disableInterrupt();
                toggleLock();
                enableInterrupt();
                command = '0';
                break;
            case '5':
                disableInterrupt();
                toggleEncrypt();
                enableInterrupt();
                command = '0';
                break;
            case '6':
                disableInterrupt();
                resetPinHolders();
                delayInMs(500);
                askForChangePin(currP, newP, confP);
                delayInMs(500);
            case '7':
                disableInterrupt();
                handleChangePin();
                enableInterrupt();
                command = '0';
                break;
            case '8':
                disableInterrupt();
                sendString(C2OKRBT);
                delayInMs(100);
                pinCount = 3;
                systemLocked();
                LCD_Clear();
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
        } else if (command == '3') {
            char c = keyPressed();
            if (c != NULL && c != '#' && c != '*') {
                if (c == '4') {
                    command = '8';
                } else {
                    command = c + 3;
                }
                if (command < '4' || command > '8') {
                    LCD_Clear();
                    LCD_String_xy(1, 0, "Wrong choice!");
                    buzzOnAndOff(1000);
                    buzzOff();
                    LCD_Clear();
                    command = '3';
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
        commandBT[index] = receiveChar();
        commandBT[index + 1] = 0;
        index = (index + 1) % 16;
        RCIF = 0;
        enableInterrupt();
        RCIF = 0;
    }
}
