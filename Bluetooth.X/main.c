/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:45
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/bluetooth.h"
#include "../eLock.X/lcd.h"

char out = '\0';

void main(void) {
    init();
    LCD_Init();
    OSCCON = 0x72;
    LCD_Clear();
    LCD_String_xy(1,0,"Receive: ");
    LCD_Command(0xC0);
    while(1) {
        sendCharacter('a');
        LCD_Char('a');
        //delayInMs(10);
        char c = receiveChar();
        LCD_Char(c);
    }
    /*char received[256] = "";
    int i = 0;
    char connected = 0;
    const char OK_CONN[] = "OK+CONN";
    const char OK_LOST[] = "OK+LOST";
    while (1) {
        char c = receiveChar();
        if (c != NULL) {
            received[i++] = c;
            if (connected) {
                LCD_Char(c);
                sendCharacter(c);
            } else if (strcmp(OK_CONN, received) == 0) {
                connected = 1;
            } else if (strcmp(OK_LOST, received) == 0) {
                connected = 0;
            }
            i = i % 256;
            c = NULL;
        }
    }*/
    
}
