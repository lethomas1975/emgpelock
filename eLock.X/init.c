/*
 * File:   init.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:38
 */


#include <xc.h>
#include "init.h"

void init_usart(void);

void init_usart(void) {
    TRISC6=0;                       /*Make Tx pin as output*/
    TRISC7=1;                       /*Make Rx pin as input*/
    SPBRG=(int)Baud_value(9600);                /*baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    TXSTA=0x20;                     /*Transmit Enable(TX) enable*/ 
    RCSTA=0x90;                     /*Receive Enable(RX) enable and serial port enable */    
}

void init(void) {
    // disabling ADC
    ADCON0bits.GO = 0;
    ADCON0bits.ADON = 0;

    ADCON1 = 0x01; // A0 use Analog
    ADCON2 = 0b10001001; // right alignment, 2TAD and FOSC/8
    ADRESH=0;
    ADRESL=0;
    
    INTCON2 = 0;

    BUZZOut = 0;
    BUZZTrisOut = 0;
    
    SolenoidOut = 0;
    SolenoidTrisOut = 0;

    // set 7-Segment as output (TRIS to 0) and reset the pins to 0
    SevenSEGOut0 = 0;
    SevenSEGOut1 = 0;
    SevenSEGOut2 = 0;
    SevenSEGTrisOut0 = 0;
    SevenSEGTrisOut1 = 0;
    SevenSEGTrisOut2 = 0;

    RGBLedOut = 0;
    RGBLedTrisOut = 0;
    
    // set the LED as output (TRIS to 0)
    LEDPin = 0;
    LEDTris = 0;
}