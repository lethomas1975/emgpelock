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

int parseCommand(const char * recv);

void main(void) {
    LCD_Init();
    OSCCON = 0x72;

    setupPin();
    setupEncrypt();
    LCD_Clear();
    while (1) {
        char e = 0;
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
                e = readEncrypt();
                char LINE2[16] = "4-";
                if (e == 0) {
                    strcat(LINE2, "D");
                } else {
                    strcat(LINE2, "E");
                }
                strcat(LINE2, " Enc 5-Rst BT");                
                LCD_String_xy(1, 0, "1-L 2-U 3-Ch PIN");
                LCD_String_xy(2, 0, LINE2);
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
            case '6':
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
                break;
            case '7':
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
        char commandBT[10] = "";
        int i = 0;
        char data = 0;
        char stop = 0;
        while (!stop) {
            char c = receiveChar();
            if (c == 0 || c == NULL) {
                RGBLedROut = 1;
            } else if (data == 0) {
                commandBT[i] = c;
                int cmd = parseCommand(commandBT);
                switch (cmd) {
                    case 1:
                        sendAppStatus();
                        stop = 1;
                        break;
                    case 2:
                        stop = 1;
                        break;
                    case 3:
                        data = 1;
                        i = -1;
                        break;
                    case 4:
                        data = 2;
                        i = -1;
                        break;
                }
            } else if (data == 1) {
                if (c == '0') {
                    sendAppStatus();
                    stop = 1;
                } else if (c == '3') {
                    command = '9';
                } else {
                    if (command == '9') {
                        if (c != '+') {
                            if (i < 3) {
                                oldP[i] = c;
                            } else if (i < 7) {
                                newP[i - 4] = c;
                            } else {
                                conP[i - 8] = c;
                            }
                            if (i == 11) {
                                stop = 1;
                            }
                        }
                    } else {
                        command = c + 2;
                        stop = 1;
                    }
                }
            } else if (data == 2) {
                oldP[i] = c;
                if (i == 3) {
                    if (checkPin(oldP)) {
                        sendString(C2OKPIN);
                    } else {
                        sendString(C2NOKPIN);
                    }
                    stop = 1;
                }
            }
            i++;
        }
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