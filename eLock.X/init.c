/*
 * File:   init.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:38
 */
#include "init.h"
#include "common.h"

void init_usart(void);
void init_interrupt(void);

void init_usart(void) {
    TRISC6 = 0;                       /*Make Tx pin as output*/
    TRISC7 = 1;                       /*Make Rx pin as input*/
    SPBRG = (int) baudValue(9600);    /*baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    TXSTA = 0x20;
    RCSTA = 0x90;                     /*Receive Enable(RX) enable and serial port enable */    
    BAUDCONbits.BRG16 = 0;
    //INTCONbits.GIE = 0;             /* Enable Global Interrupt */
    //INTCONbits.PEIE = 0;            /* Enable Peripheral Interrupt */
    //PIE1bits.RCIE = 0;              /* Enable Receive Interrupt*/
    //PIE1bits.TXIE = 0;              /* Enable Transmit Interrupt*/
}

void init_interrupt(void) {
    IPEN = 1;               // enable Interrupt
    INTCON = 0b11011000;    // enable High and Low Priority (BT and Keypad accordingy) 
    INTCON2bits.RBIP = 0;   // Keypad Interrupt low priority
    PIE1 = 0x20;            // enable RX Interrupt
    IPR1 = 0x20;            // High priority on RX Interrupt
    
}

void init(void) {
    OSCTUNE = 0b1001111;
    OSCCON = 0x72;
    // disabling ADC
    ADCON0bits.GO = 0;
    ADCON0bits.ADON = 0;
    ADCON1 = 0x0F;
    CMCON = 0x07;
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

    RGBLedROut = 0;
    RGBLedGOut = 0;
    RGBLedBOut = 0;
    RGBLedRTrisOut = 0;
    RGBLedGTrisOut = 0;
    RGBLedBTrisOut = 0;
    
    // set the LED as output (TRIS to 0)
    LEDPin = 0;
    LEDTris = 0;
    
    KeypadTrisOut = 0x70;
    KeypadIn = 0xf0;
    KeypadOut = 0xff;

    init_usart();
    //init_interrupt();
}