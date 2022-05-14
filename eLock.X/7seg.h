#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic18f4550.h>
#endif	/* XC_HEADER_TEMPLATE_H */

#ifndef SEVEN_SEG_H
#define	SEVEN_SEG_H

// functions for 7-Segment
void setSevenSegment(int count);    // display the 7-Segment on the LED

#endif