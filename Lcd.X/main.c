/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:24
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/lcd.h"

void main(void) {
    int count = 0;
    while (1) {
        display("Hello * " + count);
        delayInMs(50);
        count ++;
    }
}
