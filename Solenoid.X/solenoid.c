/*
 * File:   solenoid.c
 * Author: thomas
 *
 * Created on 14 May 2022, 18:36
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"

void main(void) {
    init();
    while (1) {
        SolenoidOut = ~SolenoidOut;
        delayInMs(100);
    }
}