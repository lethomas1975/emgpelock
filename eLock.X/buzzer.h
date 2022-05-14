#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic18f4550.h>
#endif	/* XC_HEADER_TEMPLATE_H */

#ifndef BUZZER_H
#define BUZZER_H
void buzz(void);
void buzzOff(void);
#endif
