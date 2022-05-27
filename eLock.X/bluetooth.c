/*
 * File:   bluetooth.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:51
 */


#include "init.h"
#include "bluetooth.h"

char connected = 0;
void sendCharacter(char c) {
    while (TXIF == 0);
    TXREG = c;
    //while (TRMT == 0);
}

void sendString(const char *out) {
    int i = 0;
    while (out[i] != '\0') {            
        sendCharacter(out[i]);
        i++;
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
    if (strcmp(OKCONN, recv) == 0) {
        setConnected();
        return 1;
    }
    return 0;
}

char isOkLost(const char * recv) {
    if (strcmp(OKLOST, recv) == 0) {
        setDisconnected();
        return 1;
    }
    return 0;    
}

char isConnected(void) {
    return connected;
}
void setConnected(void) {
    connected = 1;
}
void setDisconnected(void) {
    connected = 0;
}

char isC2Command(const char * recv) {
    return strcmp(C2COMMAND, recv) == 0;
}

char isC2Pin(const char * recv) {
    return (strcmp(C2PIN, recv));
}