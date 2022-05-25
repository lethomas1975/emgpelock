/*
 * File:   solenoid.c
 * Author: thomas
 *
 * Created on 14 May 2022, 18:36
 */


#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/solenoid.h"

void main(void) {
    init();
    OSCCON = 0x72;
    while (1) {
        //SolenoidOut = ~SolenoidOut;
        lock();
        delayInMs(500);
        unlock();
        delayInMs(500);
    }
}