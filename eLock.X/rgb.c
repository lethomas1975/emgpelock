/*
 * File:   rgb.c
 * Author: thomas
 *
 * Created on 31 May 2022, 22:39
 */

#include "init.h"
#include "rgb.h"

void redOn(void) {
    RGBLedROut = 1;
    RGBLedGOut = 0;
    RGBLedBOut = 0;
}

void greenOn(void) {
    RGBLedROut = 0;
    RGBLedGOut = 1;
    RGBLedBOut = 0;
}

void blueOn(void) {
    RGBLedROut = 0;
    RGBLedGOut = 0;
    RGBLedBOut = 1;
}

void rgbOff(void) {
    RGBLedROut = 0;
    RGBLedGOut = 0;
    RGBLedBOut = 0;    
}
