/*
 * File:   eeprom.c
 * Author: thomas
 *
 * Created on 14 May 2022, 20:03
 */
#include "../eLock.X/init.h"
#include "../eLock.X/eeprom.h"


void EEPROM_Write(int, char);		/* Write byte to EEPROM */
char EEPROM_Read(int);              /* Read byte From EEPROM */
void EEPROM_WriteString(int, const char*);	/* Write String to EEPROM */

const char CHECK_PIN_DELIM[] = "P=";
const char ENCRYPT[] = "E=";

/**
 * readPinFromEeprom()
 * read a 3 digit pin from the EEPROM
 * 
 * Parameters:
 *  pin: PIN holder where the PIN is returned
 * 
 * Author: Thomas Le 24/05/2022
 */
void readPinFromEeprom(char pin[4]) {
    int i = 0;
    char checkPinDelimiter[3] = "";
    
    while (i < 2) {
        checkPinDelimiter[i] = EEPROM_Read(PIN_START_ADDRESS + i);
        i++;
    }
    checkPinDelimiter[2] = 0;
    if (strcmp(CHECK_PIN_DELIM, checkPinDelimiter) == 0) { 
        i = 0;
        while (i < MAX_PIN_SIZE - 1) {
            pin[i] = EEPROM_Read(PIN_START_ADDRESS + i + 2);
            i++;
        }
        pin[3] = 0;
    }
}

/**
 * savePinToEeprom()
 * save a 3 digit pin to the EEPROM
 * 
 * Parameters:
 *  pin: PIN to save to EEPROM
 * 
 * Author: Thomas Le 24/05/2022
 */
void savePinToEeprom(const char pin[4]) {
    char toSave[6] = ""; 
    strcpy(toSave, CHECK_PIN_DELIM);
    strcat(toSave, pin);
    EEPROM_WriteString(PIN_START_ADDRESS, toSave);
}

/**
 * readEncryptFromEeprom()
 * read encryption state from EEPROM
 * 
 * Return:
 *  0 for decryption
 *  1 for encryption
 * 
 * Author: Thomas Le 24/05/2022
 */
char readEncryptFromEeprom(void) {
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

/**
 * saveEncryptToEeprom()
 * save encryption state to EEPROM
 * 
 * Return:
 *  0 for decryption
 *  1 for encryption
 * 
 * Author: Thomas Le 24/05/2022
 */
void saveEncryptToEeprom(const char encrypt) {
    char toSave[4] = "";
    strcpy(toSave, ENCRYPT);
    char toEncrp[2] = "";
    toEncrp[0] = encrypt;
    toEncrp[1] = 0;
    strcat(toSave, toEncrp);
    EEPROM_WriteString(ENCRYPT_START_ADDRESS, toSave);
}

// Basic functions

/**
 * EEPROM_Write()
 * write a character to an address in the EEPROM
 * 
 * Parameters:
 *  address: where to write the character to
 *  data: character to write to
 * 
 * Author: provided by electronicwings
 */
void EEPROM_Write (int address, char data)
{
    /*Write Operation*/
    EEADR = address;	/* Write address to the EEADR register*/
    EEDATA = data;	/* Copy data to the EEDATA register for write */
    EECON1bits.EEPGD = 0;	/* Access data EEPROM memory*/
    EECON1bits.CFGS = 0;	/* Access flash program or data memory*/
    EECON1bits.WREN = 1;	/* Allow write to the memory*/
    INTCONbits.GIE = 0;	/* Disable global interrupt*/
    
    /* Below sequence in EECON2 Register is necessary 
    to write data to EEPROM memory*/
    EECON2 = 0x55;
    EECON2 = 0xaa;
    
    EECON1bits.WR = 1;	/* Start writing data to EEPROM memory*/
    INTCONbits.GIE = 1;	/* Enable interrupt*/
    
    while (PIR2bits.EEIF == 0);/* Wait for write operation complete */
    PIR2bits.EEIF = 0;	/* Reset EEIF for further write operation */
    
}

/**
 * EEPROM_WriteString()
 * write a string to an address in the EEPROM
 * 
 * Parameters:
 *  address: where to write the character to
 *  data: string to write to
 * 
 * Author: provided by electronicwings
 */
void EEPROM_WriteString(int address, const char *data) {
    /*Write Operation for String*/
    while (*data != 0)
    {
        EEPROM_Write(address, *data);
        address++;
        data++;
    }    
}

/**
 * EEPROM_Read()
 * read a character from an address in the EEPROM
 * 
 * Parameters:
 *  address: where to read the character from
 * 
 * Author: provided by electronicwings
 */
char EEPROM_Read (int address)
{
    /*Read operation*/
    EEADR = address;	/* Read data at location 0x00*/
    EECON1bits.WREN = 0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD = 0;	/* Access data EEPROM memory*/
    EECON1bits.RD = 1;	/* To Read data of EEPROM memory set RD=1*/
    return EEDATA;
}