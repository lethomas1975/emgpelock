/*
 * File:   eeprom.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:02
 */


#include <xc.h>
#include <stdlib.h>
#include "../eLock.X/eeprom.h"

void main(void) {
    while (1) {
        char* pin = malloc(sizeof(char) * 4);
        *pin = "123";
        savePin(pin);
        char* pin2 = readPin();
        
        // check pin and pin2 are equals
    }
    
}
