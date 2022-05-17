/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 21:59
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"

void main(void) {
    init();
    
    unsigned int duty_cycle;  
    //OSCCON=0x72;         /* set internal clock to 8MHz */
    //TRISCbits.TRISC2=0;  /* Set CCP1 pin as output for PWM out */
    PR2=199;             /* load period value in PR2 register */ 
    CCPR1L=1;            /* load duty cycle */
    T2CON=0;             /* no pre-scalar,timer2 is off */
    CCP1CON=0x0C;        /* set PWM mode and no decimal value for PWM */
    TMR2=0;
    T2CONbits.TMR2ON=1;  /* Turn ON Timer2 */
    while(1)
    {

        for(duty_cycle=1;duty_cycle<199;duty_cycle++)
        {
            CCPR1L = duty_cycle;   /* load duty cycle */
            delayInMs(2);
        }
        delayInMs(500);
        
        for(duty_cycle=199;duty_cycle>1;duty_cycle--)
        {
            CCPR1L = duty_cycle;   /* load duty cycle */
            delayInMs(20);
        }
        delayInMs(500);
    }
}
