/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 21:59
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/rgb.h"

void main(void) {
    init();

    while(1) {
        rgbOff();
        redOn();
        delayInMs(500);
        rgbOff();
        greenOn();
        delayInMs(500);
        rgbOff();
        blueOn();
        delayInMs(500);
    }
}
