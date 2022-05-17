/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

//#pragma config FOSC = INTOSC_HS  	// Internal oscillator, HS used by USB.
#pragma config FOSC = INTOSCIO_EC   	// Internal oscillator, port function on RA6, EC used by USB. 
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
#pragma config WDT = OFF        // Disable watchdog timer
#pragma config XINST = OFF
#pragma config LVP = ON

// definition for Keypad
#define KeypadOut LATB
#define KeypadTrisOut TRISB


// definition for LCD
#define LCDOut LATD
#define LCDTrisOut TRISD
#define LCDE0Out LATAbits.LATA0
#define LCDE0TrisOut TRISAbits.TRISA0
#define LCDE1Out LATAbits.LATA1
#define LCDE1TrisOut TRISAbits.TRISA1

// definition for the Solenoid
#define SolenoidOut LATAbits.LATA2
#define SolenoidTrisOut TRISAbits.TRISA2

// definition for the RGB LED
#define RGBLedOut LATCbits.LATC2
#define RGBLedTrisOut TRISCbits.TRISC2

// definition for the Buzzer
#define BUZZOut LATCbits.LATC1
#define BUZZTrisOut TRISCbits.TRISC1
#define LEDPin LATCbits.LATC0       	// Define LEDPin as PORT C Pin 2 for left turn and other tests
#define LEDTris TRISCbits.TRISC0    	// Define LEDTris as TRISC Pin 2 as output mode

// definition for the 7-Segment
#define SevenSEGOut0 LATEbits.LATE0 // RB1 segment g
#define SevenSEGOut1 LATEbits.LATE1 // RB4 segment a
#define SevenSEGOut2 LATEbits.LATE2 // RB2 segment d
#define SevenSEGTrisOut0 TRISEbits.TRISE0
#define SevenSEGTrisOut1 TRISEbits.TRISE1
#define SevenSEGTrisOut2 TRISEbits.TRISE2

#define F_CPU 8000000
#define baudValue(baud_rate) (((float)(F_CPU)/(64.0f*(float)baud_rate))-1)

void init(void);

void LCD_Init(void);

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic18f4550.h>

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

