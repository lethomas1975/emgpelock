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

#ifndef ELOCK_H
#define ELOCK_H
void askPin(const char* message, char pin[MAX_PIN_SIZE]);
char checkPin(char pin[MAX_PIN_SIZE]);
//char login(void);

void handleChangePin(void);
void askForChangePin(char currPin[MAX_PIN_SIZE], char newPin[MAX_PIN_SIZE], char confPin[MAX_PIN_SIZE]);
char changePin(const char oldP[MAX_PIN_SIZE], const char newP[MAX_PIN_SIZE], const char conP[MAX_PIN_SIZE]);
char confirmPin(const char pin1[MAX_PIN_SIZE], const char pin2[MAX_PIN_SIZE]);
void handleConfirmPin(char confirmed, int *count);

void systemLocked(void);

void disableInterrupt(void);
void enableInterrupt(void);

void setupPin(void);
void setupEncrypt(void);
void toggleEncrypt(void);
void resetBT(void);

void sendEncryptStatus(void);
void sendAppStatus(void);

void displayInstruction(void);
void displayMenu(void);

void toggleLock(void);

void resetPinHolders(void);
void clearCommandString(void);

void parseChangePin(char cmd[MAX_COMMAND_SIZE]);
void parsePin(void);
void handleBTCommand(void);
int parseCommand(const char * recv);

#endif
