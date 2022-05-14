/*
 * File:   7s.c
 * Author: thomas
 *
 * Created on 27 March 2022, 17:51
 */
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/7Seg.h"

void main(void) {
    init();
    
    while (1) {
        for (int i = 0; i < 4; i++) {
            setSevenSegment(i);
            delayInMs(100);
        }
    }
}
