/*
 * File:   rgb.c
 * Author: thomas
 *
 * Created on 31 May 2022, 22:39
 */

#include "init.h"
#include "rgb.h"

/**
 * redOn()
 * turn on RED light
 * 
 * Author: Thomas Le 31/05/2022
 */
void redOn(void) {
    RGBLedROut = 1;
    RGBLedGOut = 0;
    RGBLedBOut = 0;
}

/**
 * greenOn()
 * turn on GREEN light
 * 
 * Author: Thomas Le 31/05/2022
 */
void greenOn(void) {
    RGBLedROut = 0;
    RGBLedGOut = 1;
    RGBLedBOut = 0;
}

/**
 * blueOn()
 * turn on BLUE light
 * 
 * Author: Thomas Le 31/05/2022
 */
void blueOn(void) {
    RGBLedROut = 0;
    RGBLedGOut = 0;
    RGBLedBOut = 1;
}

/**
 * rgbOff()
 * turn off LED
 * 
 * Author: Thomas Le 31/05/2022
 */
void rgbOff(void) {
    RGBLedROut = 0;
    RGBLedGOut = 0;
    RGBLedBOut = 0;    
}
