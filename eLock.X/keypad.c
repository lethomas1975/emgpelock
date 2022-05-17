/*
 * File:   keypad.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:34
 */
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/keypad.h"

const int ROW = 4;
const int COLUMN = 3;

char keyPressed() {
    char mask = 1;
    for (int row = 0; row < ROW; row++) {
        KeypadOut = 0;
        char tmp = KeypadOut;
        if (row > 0) {
            mask = mask << 1;
        }
        KeypadOut |= mask;
        tmp = KeypadOut;
        delayInMs(20);
        char columns = (KeypadOut & 0b01110000) >> 4;
        if (columns == 1) {
            return row < 3 ? (1 + 3 * row) : '*';
        }
        if (columns == 2) {
            return row < 3 ? (2 + 3 * row) : 0;
        }
        if (columns == 4) {
            return row < 3 ? (3 + 3 * row) : '#';
        }
    }
    KeypadOut = 0;    
    return NULL; // for scenario when nothing was pressed
}
