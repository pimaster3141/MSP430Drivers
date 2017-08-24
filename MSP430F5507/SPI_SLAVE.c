/*
 * SPI_SLAVE.c (1.1)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *
 */

#include <msp430f5507.h>
#include "SPI_SLAVE.h"


// volatile unsigned char SPIRXBuffer[RX_BUFFER_SIZE];		// structure to hold received SPI data
// volatile unsigned char SPIRXBufferPointer = 0; 			// pointer to SPI data

static volatile unsigned char SPITXBuffer[TX_BUFFER_SIZE];		// structure to hold transmit SPI data (circular buffer)
static volatile unsigned char SPITXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char SPITXBufferEndPointer = 0;		// pointer to first empty slot in buffer

#pragma vector=USCI_B1_VECTOR		//SPI interrupt
__interrupt void SPI_ISR(void)
{
	switch(UCB1IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// SPIRXBuffer[SPIRXBufferPointer] = UCB1RXBUF;	// store character in string array
			// SPIRXBufferPointer++;		// increment pointer
			SPIReceiveAction(UCB1RXBUF);
			break;

		case 4:		// TX interrupt
			UCB1TXBUF = SPITXBuffer[SPITXBufferSendoutPointer];
			SPITXBufferSendoutPointer++;
//			SPITXBufferSendoutPointer = (SPITXBufferSendoutPointer+1) % TXBufferSize;
			if(SPITXBufferSendoutPointer >= TX_BUFFER_SIZE)
				SPITXBufferSendoutPointer = 0;
			if(SPITXBufferSendoutPointer == SPITXBufferEndPointer)
				UCB1IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}

void SPISetup(void)
{
	UCB1CTL1 |= UCSWRST;				// Hold SPI
	UCB1CTL0 |= UCCKPH + UCMSB + UCMODE_2 + UCSYNC;	// MSB first, MasterMode, Sync
	UCB1IE |= UCRXIE;					// Enable TXinterrupt

	P4DIR &= ~(BIT1 + BIT3);			// Set pin for input direction
	P4SEL |= BIT1 + BIT2 + BIT3; 		// Set pins for secondary functions (SPI)

	UCB1CTL1 &= ~UCSWRST;				// Bring out of reset

	UCB1IE |= UCRXIE;					// Enable RX interrupts
}

void SPISendByte(char byte)
{
	SPITXBuffer[SPITXBufferEndPointer] = byte;
	SPITXBufferEndPointer++;
	if(SPITXBufferEndPointer >= TX_BUFFER_SIZE)
		SPITXBufferEndPointer = 0;
	UCB1IE |= UCTXIE;
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
	UCB1IE |= UCTXIE;
	return;
}

// void clearSPIRXBuffer(void)
// {
// 	while(UCB1STAT & UCBUSY);
// 	SPIRXBufferPointer = 0;
// }

// void readSPIRXBuffer(char *outputBuffer, char length)
// {
// 	char i;
// 	for (i = 0; i < length; i++)
// 		outputBuffer[i] = SPIRXBuffer[i];
// 	return;
// }

