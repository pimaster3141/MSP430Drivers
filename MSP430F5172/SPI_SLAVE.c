/*
 * SPI_SLAVE.c (1.1)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *		
 *	Revised 8/31/16 - Update for F5172 Ports
 *
 */

#include <msp430f5172.h>
#include "SPI_SLAVE.h"


// volatile unsigned char SPIRXBuffer[RX_BUFFER_SIZE];		// structure to hold received SPI data
// volatile unsigned char SPIRXBufferPointer = 0; 			// pointer to SPI data

static volatile unsigned char SPITXBuffer[TX_BUFFER_SIZE];		// structure to hold transmit SPI data (circular buffer)
static volatile unsigned char SPITXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char SPITXBufferEndPointer = 0;		// pointer to first empty slot in buffer

#pragma vector=USCI_B0_VECTOR		//SPI interrupt
__interrupt void SPI_ISR(void)
{
	switch(UCB0IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// SPIRXBuffer[SPIRXBufferPointer] = UCB0RXBUF;	// store character in string array
			// SPIRXBufferPointer++;		// increment pointer
			SPIReceiveAction(UCB0RXBUF);
			break;

		case 4:		// TX interrupt
			UCB0TXBUF = SPITXBuffer[SPITXBufferSendoutPointer];
			SPITXBufferSendoutPointer++;
//			SPITXBufferSendoutPointer = (SPITXBufferSendoutPointer+1) % TXBufferSize;
			if(SPITXBufferSendoutPointer >= TX_BUFFER_SIZE)
				SPITXBufferSendoutPointer = 0;
			if(SPITXBufferSendoutPointer == SPITXBufferEndPointer)
				UCB0IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}

void SPISetup(void)
{
	UCB0CTL1 |= UCSWRST;				// Hold SPI
	UCB0CTL0 |= UCCKPH + UCMSB + UCMODE_2 + UCSYNC;	// MSB first, MasterMode, Sync
	UCB0IE |= UCRXIE;					// Enable TXinterrupt

	P1DIR &= ~(BIT4 + BIT3 + BIT0);			// Set pins for input direction (SCLK/SIMO)
	P1DIR |= BIT5;						// Set pins for output direction (SOMI)
	P1SEL |= BIT3 + BIT4 + BIT5; 		// Set pins for secondary functions (SPI)

	UCB0CTL1 &= ~UCSWRST;				// Bring out of reset

	UCB0IE |= UCRXIE;					// Enable RX interrupts
}

void SPISendByte(char byte)
{
	SPITXBuffer[SPITXBufferEndPointer] = byte;
	SPITXBufferEndPointer++;
	if(SPITXBufferEndPointer >= TX_BUFFER_SIZE)
		SPITXBufferEndPointer = 0;
	UCB0IE |= UCTXIE;
}

void SPISendArray(char *TxArray, unsigned char ArrayLength)
{
	while(ArrayLength--)
	{
		SPITXBuffer[SPITXBufferEndPointer] = *TxArray;
		TxArray++;
		SPITXBufferEndPointer++;
		if(SPITXBufferEndPointer >= TX_BUFFER_SIZE)
			SPITXBufferEndPointer = 0;
	}
	UCB0IE |= UCTXIE;
	return;
}

char isCS()
{
	return P1IN & BIT0;
}

// void clearSPIRXBuffer(void)
// {
// 	while(UCB0STAT & UCBUSY);
// 	SPIRXBufferPointer = 0;
// }

// void readSPIRXBuffer(char *outputBuffer, char length)
// {
// 	char i;
// 	for (i = 0; i < length; i++)
// 		outputBuffer[i] = SPIRXBuffer[i];
// 	return;
// }

