/*
 * File:   bluetooth.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:51
 */
#include "init.h"
#include "common.h"
#include "bluetooth.h"

/**
 * sendCharacter()
 * transmit a character via TX
 *
 * Parameters:
 *  c: character to send
 * 
 * Author: Suresh (check reference from report on UART)
 */
void sendCharacter(char c) {
    TXREG = c;
    while (!TRMT);
}

/**
 * sendString()
 * send a string
 * 
 * Parameters:
 *  out: string to send
 * 
 * Author: Suresh (check reference from report on UART)
 */
void sendString(const char *out) {
    TRMT = 1;
    while (*out != '\0') {
        sendCharacter(*out);
        out++;
    }
}

/**
 * receiveChar()
 * read a character on TRX
 * 
 * Author: Suresh (check reference from report on UART)
 */
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

/**
 * isOkConn()
 * check if string is OK+CONN
 * 
 * Parameters:
 *  recv: string to check
 * 
 * Return:
 *  0 if false, 1 if true
 * Author: Thomas Le 30/05/2022
 */
char isOkConn(const char * recv) {
    return (strstartwith((char*)recv, (char*)OKCONN) == 1 || strcmp(recv, OK) == 0);
}

/**
 * isOkLost()
 * check if string is OK+LOST
 * 
 * Parameters:
 *  recv: string to check
 * 
 * Return:
 *  0 if false, 1 if true
 * Author: Thomas Le 30/05/2022
 */
char isOkLost(const char * recv) {
    return (strstartwith((char*)recv, (char*)OKLOST) == 1);    
}

/**
 * isC2Command()
 * check if string is a command starting with C2+
 * 
 * Parameters:
 *  recv: string to check
 * 
 * Return:
 *  0 if false, 1 if true
 * Author: Thomas Le 30/05/2022
 */
char isC2Command(const char * recv) {
    return strstartwith((char*)recv, (char*)C2COMMAND) == 1;
}

/**
 * isC2Pin()
 * check if string is a PIN command starting with C2PIN+
 * 
 * Parameters:
 *  recv: string to check
 * 
 * Return:
 *  0 if false, 1 if true
 * Author: Thomas Le 30/05/2022
 */
char isC2Pin(const char * recv) {
    return strstartwith((char*)recv, (char*)C2PIN) == 1;
}
