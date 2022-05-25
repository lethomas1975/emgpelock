/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:24
 */


#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/lcd.h"

void main(void) {
    LCD_Init();
    OSCCON = 0x72;
    
    int i = 0;
    while(1) {
        LCD_String_xy(1,(16 - 5)/2,"Hello");
        char c = '0' + i;
        char tmp[11] = "Team C2: ";
        tmp[9] = c;
        tmp[10] = 0;
        LCD_String_xy(2,(16 - 10)/2, tmp);
        delayInMs(100);
        i = (i + 1) %10;
    }
}
