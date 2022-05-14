/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 21:59
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"

void main(void) {
    init();
    while(1) {
        RGBLedOut = ~RGBLedOut;
        delayInMs(100);
    }
}
