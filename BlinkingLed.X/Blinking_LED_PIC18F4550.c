//tested working

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>                     //xc8 compiler header file.
#include <pic18f4550.h>

//#pragma config FOSC = INTOSC_HS  	// Internal oscillator, HS used by USB.
#pragma config FOSC = INTOSCIO_EC   	// Internal oscillator, port function on RA6, EC used by USB. 
#pragma config WDT = OFF            	// Disable watchdog timer

// change the port and its pin number depending on your own design.
#define LEDPin LATCbits.LATC2       	// Define LEDPin as PORT D Pin 0
#define LEDTris TRISCbits.TRISC2    	// Define LEDTris as TRISD Pin 0 as output mode

#define _XTAL_FREQ 8000000              // for __delay_ms() function

void main(void) {

    LEDTris = 0;	//Set LED Pin data direction to OUTPUT
    LEDPin = 1;	//Set LED Pin

    while(1) {
        LEDPin = ~LEDPin;	// Toggle LED Pin
        __delay_ms(10);
    }
}
