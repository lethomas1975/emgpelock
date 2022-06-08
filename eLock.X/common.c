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

void delayInMs(int ms) {
    if (ms/10 > 1) {
        for (int i = 0; i < ms / 10; i++) {
            __delay_ms(10);
        }
    } else {
        __delay_ms(1);
    }
}

void delayInUs(int us) {
    if (us/10 > 1) {
        for (int i = 0; i < us / 10; i++) {
            __delay_us(10);
        }
    } else {
        __delay_us(1);
    }
}

char strstartwith(const char* source, const char* startwith) {
    if (*startwith == 0 && *source == 0) {
        return 0;
    }
    while (*startwith != 0) {
        if (*startwith != *source) {
            return 0;
        }
        *startwith++;
        *source++;
        if (*startwith != 0 && *source == 0) {
            return 0;
        }
    }
    return 1;
}

void clearString(char* source, int length) {
    if (source != NULL) {
        for (int i = 0; i < length; i++) {
            source[i] = 0;
        }
    }
}

void decrypt(char toDecrypt[4]) {
    char tmp[4] = "";
    for (int i = 0; i < 3; i++) {
        tmp[i] = toDecrypt[i] - 97 - i;
    }
    tmp[3] = 0;
    strcpy(toDecrypt, tmp);
}