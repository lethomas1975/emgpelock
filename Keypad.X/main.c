/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:36
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/keypad.h"

void main(void) {
    while(1) {
        char* pin = readPin();
        
        // compare that pin is expected
    }
}
