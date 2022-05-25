/*
 * File:   solenoid.c
 * Author: thomas
 *
 * Created on 22 May 2022, 22:18
 */
#include "init.h"
#include "common.h"
#include "solenoid.h"

void lock(void) {
    SolenoidOut = 1;
}

void unlock(void) {
    SolenoidOut = 0;
}

