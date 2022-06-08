/*
 * File:   buzzer.c
 * Author: thomas
 *
 * Created on 27 March 2022, 18:15
 */
#include "init.h"
#include "common.h"
#include "buzzer.h"

/**
 * buzz()
 * turn on the buzzer
 * 
 * Author: Lili Zhang 28/03/2022
 */
void buzz(void) {
    BUZZOut = 1;
}

/**
 * buzzOff()
 * turn off the buzzer
 * 
 * Author: Lili Zhang 28/03/2022
 */
void buzzOff(void) {
    BUZZOut = 0;
}

/**
 * buzzOnAndOff()
 * turn on and off the buzzer with a delay
 * 
 * Parameters:
 *  delay: time to delay
 *
 * Author: Thomas Le 23/05/2022
 */
void buzzOnAndOff(int delay) {
    buzz();
    delayInMs(delay);
    buzzOff();
}