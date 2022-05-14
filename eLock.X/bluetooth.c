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
}

void sendString(const char *out) {
    while (*out!='\0') {            
        sendCharacter(*out);
        out++;
    }    
}

char receiveChar() {
    while (RCIF == 0);
    return RCREG;
}
