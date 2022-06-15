/*
 * File:   zebra.c
 * Author: thomas
 *
 * Created on 27 March 2022, 18:43
 */

#include <math.h>

#include "init.h"
#include "common.h"
#include "7seg.h"

// private function to set each segment of the 7-Segment display. prototype not defined in header but here
void setSegmentByBit(unsigned char a, unsigned char d, unsigned char g);

/**
 * setSegmentByBit()
 * setting each segment its value
 * 
 * Parameters:
 *  a: segment a either 0 or 1
 *  d: segment d either 0 or 1
 *  g: segment g either 0 or 1
 * 
 * Author: Lili Zhang 27/03/2022
 */
void setSegmentByBit(unsigned char a, unsigned char d, unsigned char g) {
    SevenSEGOut0 = d;
    SevenSEGOut1 = g;
    SevenSEGOut2 = a;
}

// a | d | g | display
// 1 | 0 | 0 | I in horizontal
// 1 | 0 | 1 | II in horizontal
// 1 | 1 | 1 | III in horizontal 
/**
 * setSevenSegment()
 * display the count on the display
 * 
 * Parameters:
 *  display: value to display
 * 
 * Author: Lili Zhang 27/03/2022
 */
void setSevenSegment(int display) {
    switch (display) {
        case 0:
            setSegmentByBit(1, 0, 0);
            break;
        case 1:
            setSegmentByBit(1, 0, 1);
            break;
        case 2:
            setSegmentByBit(1, 1, 1);
            break;
        default:
            clearSevenSegment();
            break;
    }
}

/**
 * clearSevenSegment()
 * clearing the 7-segment from displaying anything
 * Author: Thomas Le 30/05/2022
 */
void clearSevenSegment(void) {
    setSegmentByBit(0, 0, 0);    
}