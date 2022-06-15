/*
 * File:   solenoid.c
 * Author: thomas
 *
 * Created on 22 May 2022, 22:18
 */
#include "init.h"
#include "solenoid.h"

/**
 * isLocked()
 * status of the solenoid, lock or unlock position
 * 
 * Return:
 *  0 for unlocked
 *  1 for locked
 * 
 * Author: Thomas Le 22/05/2022
 */
char isLocked(void) {
    return SolenoidOut == 1;
}

/**
 * lock()
 * set solenoid to lock position
 * 
 * Author: Thomas Le 22/05/2022
 */
void lock(void) {
    SolenoidOut = 1;
}

/**
 * unlock()
 * set solenoid to unlock position
 * 
 * Author: Thomas Le 22/05/2022
 */
void unlock(void) {
    SolenoidOut = 0;
}

