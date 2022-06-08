/*
 * File:   bluetooth.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:51
 */


#include "init.h"
#include "bluetooth.h"
#include "lcd.h"
#include "common.h"
#include "7seg.h"

void sendCharacter(char c) {
    TXREG = c;
    while (!TRMT);
}

void sendString(const char *out) {
    TRMT = 1;
    while (*out != '\0') {
        sendCharacter(*out);
        out++;
    }
}

char receiveChar() {
#ifndef INTEGRATED
    while (RCIF == 0);
#endif
    if(RCSTAbits.OERR) {
        CREN = 0;
        NOP();
        CREN = 1;
    }
    char c = RCREG;
    return c;
}

char isOkConn(const char * recv) {
    return (strstartwith(recv, OKCONN) == 1 || strcmp(recv, OK) == 0);
}

char isOkLost(const char * recv) {
    return (strstartwith(recv, OKLOST) == 1);    
}

char isC2Command(const char * recv) {
    return strstartwith(recv, C2COMMAND) == 1;
}

char isC2Pin(const char * recv) {
    return strstartwith(recv, C2PIN) == 1;
}