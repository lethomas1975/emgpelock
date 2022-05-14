/*
 * File:   main.c
 * Author: thomas
 *
 * Created on 13 April 2022, 00:45
 */


#include <xc.h>
#include "../eLock.X/init.h"
#include "../eLock.X/common.h"
#include "../eLock.X/bluetooth.h"

void main(void) {
    init();
    
    char data_in;
    LEDPin = 0;
    
    delayInMs(50);
    while (1) {
        data_in = receiveChar();
        if (data_in == '1') {   
            LEDPin = 0;
            sendString("LED_ON");
        } else if (data_in == '2') {
            LEDPin = 1;
            sendString("LED_OFF");
        } else {
            sendString(" select 1 or 2");
        }
        delayInMs(100);
    }
}
