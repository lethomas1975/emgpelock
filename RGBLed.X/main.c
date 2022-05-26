/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 21:59
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"


void main(void) {
    init();

    while(1) {
        RGBLedROut = 1;
        RGBLedGOut = 0;
        RGBLedBOut = 0;
        delayInMs(500);
        RGBLedROut = 0;
        RGBLedGOut = 1;
        RGBLedBOut = 0;
        delayInMs(500);
        RGBLedROut = 0;
        RGBLedGOut = 0;
        RGBLedBOut = 1;
        delayInMs(500);
        RGBLedROut = 1;
        RGBLedGOut = 1;
        RGBLedBOut = 0;
        delayInMs(500);
        RGBLedROut = 1;
        RGBLedGOut = 0;
        RGBLedBOut = 1;
        delayInMs(500);
        RGBLedROut = 0;
        RGBLedGOut = 1;
        RGBLedBOut = 1;
        delayInMs(500);
    }
}
