/*
 * SPI_MASTER.h (1.1)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	Revised 6/15/16 - Rename to SPI_MASTER
 */

#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

// Define structures to buffer muliple bytes at once
#define TX_BUFFER_SIZE 32u
#define RX_BUFFER_SIZE 32u
// the idea here is that this structure will hold the data going to and from the SPI
// and will be consumed/produced by the SPI ISR such that a higher level function will
// be able to perform muli-byte operations asyncronously


// where everything is connected for the chip select lines
#define CMS3x0_1 	0x01
#define CMS3x0_2 	0x02
#define CMS3x0_3 	0x04
#define LPS25HB 	0x08
#define LIS3MDL 	0x10
#define MPU3300 	0x20


/**
 * Sets up SPI - 2MHz clk
 */
void setupSPI(void);

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
void clearSPIRXBuffer(void);

/**
 * returns a number of bytes from the buffer
 * @param
 * 	char *outputBuffer - array for data to be added to
 * 	char length - number of bytes to read
 */
void readSPIRXBuffer(char *outputBuffer, char length);

/**
 * asserts the chip select line for the given chip
 * @param
 * 	unsigned char CS - device to be selected
 */
void SELECT(unsigned char CS);

/**
 * deasserts the chip select line for the given chip
 * @param
 * 	unsigned char CS - device to be deselected
 */
void DESELECT(unsigned char CS);


#endif /* SPI_MASTER_H_ */
