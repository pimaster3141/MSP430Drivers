/*
 * SPI_SLAVE.h (1.1)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *
 *	Revised 6/15/16 - Rename to SPI_SLAVE
 */

#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

// Define structures to buffer muliple bytes at once
#define TX_BUFFER_SIZE 32u
// #define RX_BUFFER_SIZE 32u
// the idea here is that this structure will hold the data going to and from the SPI
// and will be consumed/produced by the SPI ISR such that a higher level function will
// be able to perform muli-byte operations asyncronously


/**
 * Sets up SPI
 */
void SPISetup(void);

/**
 * adds a single byte to the buffer to be sent out via SPI
 * @param
 * 	char byte - byte to be sent
 */
void SPISendByte(char byte);

/**
 * adds an array of data to the buffer to be sent out via SPI
 * @param
 * 	char *TxArray - pointer to array to be sent
 * 	unsigned char ArrayLength - length of the array to be sent
 */
void SPISendArray(char *TxArray, unsigned char ArrayLengh);

/**
 * clears the input RX multibyte buffer
 */
// void clearSPIRXBuffer(void);

/**
 * returns a number of bytes from the buffer
 * @param
 * 	char *outputBuffer - array for data to be added to
 * 	char length - number of bytes to read
 */
// void readSPIRXBuffer(char *outputBuffer, char length);

 void SPIReceiveAction(unsigned char byte);


#endif /* SPI_SLAVE_H_ */
