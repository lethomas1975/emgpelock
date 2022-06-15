/*
 * File:   common.c
 * Author: thomas
 *
 * Created on 01 April 2022, 04:53
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic18f4550.h>

#include "common.h"

/**
 * delayInMs()
 * delay in millisecond
 * 
 * Parameters:
 *  ms: millisecond to delay
 * 
 * Author: Thomas Le 01/04/2022
 */
void delayInMs(int ms) {
    if (ms/10 > 1) {
        for (int i = 0; i < ms / 10; i++) {
            __delay_ms(10);
        }
    } else {
        __delay_ms(1);
    }
}

/**
 * delayInUs()
 * delay in microsecond
 * 
 * Parameters:
 *  us: microsecond to delay
 * 
 * Author: Thomas Le 01/04/2022
 */
void delayInUs(int us) {
    if (us/10 > 1) {
        for (int i = 0; i < us / 10; i++) {
            __delay_us(10);
        }
    } else {
        __delay_us(1);
    }
}

/**
 * strstartwith()
 * check if string start with another string
 * 
 * Parameters:
 *  source: string to check if prefix with the other parameter
 *  startwith: string to search for at the beginning of the source.
 * 
 * Return:
 *  0 is false
 *  1 is true
 * Author: Thomas Le 01/04/2022
 */
char strstartwith(char* source, char* startwith) {
    if (*startwith == 0 && *source == 0) {
        return 0;
    }
    while (*startwith != 0) {
        if (*startwith != *source) {
            return 0;
        }
        startwith++;
        source++;
        if (*startwith != 0 && *source == 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * clearString()
 * clear a string. set 0 to all indices until length
 * 
 * Parameters:
 *  source: string to clear
 *  length: length of the string
 * 
 * Author: Thomas Le 30/05/2022
 */
void clearString(char* source, int length) {
    if (source != NULL) {
        for (int i = 0; i < length; i++) {
            source[i] = 0;
        }
    }
}

/**
 * decrypt()
 * decrypt a string
 * 
 * Parameters:
 *  toDecrypt: string of 3+1 length to decrypt. the decryption is contained in the same string
 * 
 * Author: Thomas Le 24/05/2022
 */
void decrypt(char toDecrypt[4]) {
    char tmp[4] = "";
    for (int i = 0; i < 3; i++) {
        tmp[i] = toDecrypt[i] - 97 - i;
    }
    tmp[3] = 0;
    strcpy(toDecrypt, tmp);
}