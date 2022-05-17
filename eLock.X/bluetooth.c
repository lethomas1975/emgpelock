/*
 * File:   bluetooth.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:51
 */


#include <xc.h>
#include "bluetooth.h"

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
    while (RCIF == 0);
    if(RCSTAbits.OERR) {
        CREN = 0;
        NOP();
        CREN = 1;
    }
    char c = RCREG;
    return c;
}
