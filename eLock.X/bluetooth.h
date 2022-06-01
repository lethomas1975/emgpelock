/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

void sendCharacter(char);
void sendString(const char *);
char receiveChar();

char isOkConn(const char *);
char isOkLost(const char *);
char isConnected(void);
void setConnected(void);
void setDisconnected(void);

char isC2Command(const char *);
char isC2Pin(const char *);

const char OK[] = "OK";
const char OKCONN[] = "OK+CONN";
const char OKLOST[] = "OK+LOST";
const char C2COMMAND[4] = "C2+";
const char C2PIN[7] = "C2PIN+";

const char C2OKSL[] = "C2OK+SL";
const char C2OKSU[] = "C2OK+SU";
const char C2OKE[] = "C2OK+E";
const char C2OKCP[] = "C2OK+CP";
const char C2NOKCP[] = "C2NOK+CP";
const char C2OKRBT[] = "C2OK+RBT";

const char C2OKPIN[] = "C2OK+PIN";
const char C2NOKPIN[] = "C2NOK+PIN";

const char C2SYSTLOCK[] = "C2SYST+LOCK";
const char C2NOK[] = "C2NOK";
#endif

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic18f4550.h>
#endif	/* XC_HEADER_TEMPLATE_H */

