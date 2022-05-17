/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:45
 */

#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/bluetooth.h"
#include "../eLock.X/lcd.h"

char out;

void main(void) {
    LCD_Init();
    LCD_String_xy(1,0,"Receive: ");
    LCD_Command(0xC0);
    init();
    while (1);
}

void interrupt ISR()
{
      while(RCIF==0);
      out=RCREG;	/* Copy received data from RCREG register */
      LCD_Char(out);
      while(TXIF==0);
      TXREG=out;	/* Transmit received(ECHO) to TXREG register */
}