/*
 * File:   eeprom.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:03
 */
#include "../eLock.X/eeprom.h"

const char CHECK_PIN[] = "P=";
const char ENCRYPT[] = "E=";

void saveEncrypt(const char encrypt) {
    char toSave[4] = "";
    strcpy(toSave, ENCRYPT);
    strcat(toSave, &encrypt);
    EEPROM_WriteString(ENCRYPT_START_ADDRESS, toSave);
}

char readEncrypt(void) {
    int i = 0;
    char checkEncrypt[3] = "";
    
    while (i < 2) {
        checkEncrypt[i] = EEPROM_Read(ENCRYPT_START_ADDRESS + i);
        i++;
    }
    checkEncrypt[2] = 0;    
    if (strcmp(ENCRYPT, checkEncrypt) == 0) {
        return EEPROM_Read(ENCRYPT_START_ADDRESS + i);
    }
    return NULL;
}

void savePin(const char* pin) {
    char toSave[6] = ""; 
    strcpy(toSave, CHECK_PIN);
    strcat(toSave, pin);
    EEPROM_WriteString(PIN_START_ADDRESS, toSave);
}

void readPin(char* pin) {
    int i = 0;
    char checkPin[3] = "";
    
    while (i < 2) {
        checkPin[i] = EEPROM_Read(PIN_START_ADDRESS + i);
        i++;
    }
    checkPin[2] = 0;
    if (strcmp(CHECK_PIN, checkPin) == 0) { 
        i = 0;
        while (i < MAX_PIN_SIZE - 1) {
            *pin = EEPROM_Read(PIN_START_ADDRESS + i + 2);
            *pin++;
            i++;
        }
        *pin = 0;
    }
}

void EEPROM_Write (int address, char data)
{
    /*Write Operation*/
    EEADR=address;	/* Write address to the EEADR register*/
    EEDATA=data;	/* Copy data to the EEDATA register for write */
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.CFGS=0;	/* Access flash program or data memory*/
    EECON1bits.WREN=1;	/* Allow write to the memory*/
    INTCONbits.GIE=0;	/* Disable global interrupt*/
    
    /* Below sequence in EECON2 Register is necessary 
    to write data to EEPROM memory*/
    EECON2=0x55;
    EECON2=0xaa;
    
    EECON1bits.WR=1;	/* Start writing data to EEPROM memory*/
    INTCONbits.GIE=1;	/* Enable interrupt*/
    
    while(PIR2bits.EEIF==0);/* Wait for write operation complete */
    PIR2bits.EEIF=0;	/* Reset EEIF for further write operation */
    
}

void EEPROM_WriteString(int address, char *data)
{
    /*Write Operation for String*/
    while(*data!=0)
    {
        EEPROM_Write(address,*data);
        address++;
        *data++;
    }    
}

char EEPROM_Read (int address)
{
    /*Read operation*/
    EEADR=address;	/* Read data at location 0x00*/
    EECON1bits.WREN=0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.RD=1;	/* To Read data of EEPROM memory set RD=1*/
    return(EEDATA);
}