/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 21:59
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"

const int low = 28;
const int high = 56;
const int reset = 1;
int red[25];
int green[25];
int blue[25];

void init_red();
void init_green();
void init_blue();

void init_red() {
    // green
    for (int i = 0; i < 8; i++) {
        red[i] = low;
    }
    // red
    for (int i = 8; i < 16; i++) {
        red[i] = high;
    }
    // blue
    for (int i = 16; i < 24; i++) {
        red[i] = low;
    }
}

void init_green() {
    // green
    for (int i = 8; i < 16; i++) {
        green[i] = high;
    }
    // red
    for (int i = 0; i < 8; i++) {
        green[i] = low;
    }
    // blue
    for (int i = 16; i < 24; i++) {
        green[i] = low;
    }
}

void init_blue() {
    // green
    for (int i = 16; i < 24; i++) {
        blue[i] = low;
    }
    // red
    for (int i = 0; i < 8; i++) {
        blue[i] = low;
    }
    // blue
    for (int i = 8; i < 16; i++) {
        blue[i] = high;
    }
}

void main(void) {
    init();
    init_red();
    init_green();
    init_blue();

    unsigned int duty_cycle;  
    OSCCON=0x72;         /* set internal clock to 8MHz */
    TRISCbits.TRISC2=0;  /* Set CCP1 pin as output for PWM out */
    PR2=15;             /* load period value in PR2 register */ 
    CCPR1L=1;            /* load duty cycle */
    T2CON=0;             /* no pre-scalar,timer2 is off */
    CCP1CON=0x0C;        /* set PWM mode and no decimal value for PWM */
    TMR2=0;
    T2CONbits.TMR2ON=1;  /* Turn ON Timer2 */
    
    while(1) {
        CCPR1L = 1;
        delayInUs(50);
        for (int i = 0; i< 24; i++) {
            CCPR1L = (PR2 + 1) * red[i] * 100;
            delayInUs(1);
        }
    }
}
