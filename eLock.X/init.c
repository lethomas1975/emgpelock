/*
 * File:   init.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:38
 */
#include "init.h"
#include "common.h"

void init_usart(void);

void init_usart(void) {
    TRISC6 = 0;                       /*Make Tx pin as output*/
    TRISC7 = 1;                       /*Make Rx pin as input*/
    SPBRG=(int)baudValue(9600);       /*baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    TXSTAbits.CSRC = 0;
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 0;
    RCSTA=0x90;                     /*Receive Enable(RX) enable and serial port enable */    
    //INTCONbits.GIE = 0;	/* Enable Global Interrupt */
    //INTCONbits.PEIE = 0;/* Enable Peripheral Interrupt */
    //PIE1bits.RCIE = 0;	/* Enable Receive Interrupt*/
    //PIE1bits.TXIE = 0;	/* Enable Transmit Interrupt*/
}

void init(void) {
    OSCCON=0x72;
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
    PR2=199;             /* load period value in PR2 register */ 
    CCPR1L=1;            /* load duty cycle */
    T2CON=0;             /* no pre-scalar,timer2 is off */
    CCP1CON=0x0C;        /* set PWM mode and no decimal value for PWM */
    TMR2=0;
    T2CONbits.TMR2ON=1;  /* Turn ON Timer2 */
    
    // set the LED as output (TRIS to 0)
    LEDPin = 0;
    LEDTris = 0;
    
    init_usart();
}