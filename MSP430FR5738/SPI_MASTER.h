/*
 * SPI_MASTER.h (1.1)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	Revised 6/15/16 - Rename to SPI_MASTER
 *	Refised 9/1/16  - Added Checks for SPI_Slave
 */

#ifndef SPI_MASTER_H_
#ifdef SPI_SLAVE_H_
#error Cannot Define Slave, Master Concurrently
#endif
#define SPI_MASTER_H_

// Define structures to buffer muliple bytes at once
#define TX_BUFFER_SIZE 32u

// the idea here is that this structure will hold the data going to and from the SPI
// and will be consumed/produced by the SPI ISR such that a higher level function will
// be able to perform muli-byte operations asyncronously


/**
 * Sets up SPI - 2MHz clk
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

void SPISendByteBlocking(char byte);
char SPITransactionBlocking(char byte);


void SPIReceiveAction(unsigned char byte);


#endif /* SPI_MASTER_H_ */
