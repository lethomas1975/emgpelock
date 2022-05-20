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
    char mask = 0xfe;
    char result = NULL;
    char found = 0;
    for (int row = 0; row < ROW; row++) {
        KeypadOut = 0xff;
        char tmp = KeypadOut;
        if (row > 0) {
            mask = (mask << 1) + 1;
        }
        char rCol = KeypadIn & 0xf0;
        char rRow = (KeypadOut &= mask);
        char newRCol1 = KeypadIn & 0xf0;
        delayInMs(20);
        char newRCol2 = KeypadIn & 0xf0;
        if (rCol != newRCol1 || rCol != newRCol2) {
            char newR;
            if (rCol != newRCol1) {
                newR = newRCol1;
            } else {
                newR = newRCol2;
            }
            char columns = ~((newR & 0b01110000) >> 4);
            if ((columns & 0b00000001) == 1) {
                result = row < 3 ? '0' + (1 + 3 * row) : '*';
                found = 1;
            } else if ((columns & 0b00000010) == 2) {
                result = row < 3 ? '0' + (2 + 3 * row) : '0';
                found = 1;
            } else  if ((columns & 0b00000100) == 4) {
                result = row < 3 ? '0' + (3 + 3 * row) : '#';
                found = 1;
            } else {
                found = 0;
            }
        }
        if (found == 1) {
            break;
        }
    }
    KeypadOut = 1;
    return result;
}
