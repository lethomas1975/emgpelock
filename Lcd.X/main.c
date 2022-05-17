/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:24
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/lcd.h"

void main(void) {
    LCD_Init();                   
    while(1) {
        LCD_String_xy(1,5,"Hello");
        LCD_String_xy(2,0,"Team C2");        
    }
}
