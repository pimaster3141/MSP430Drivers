/*
 * SPI_MASTER.c (1.1)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	Revised 6/15/16 - Rename to SPI_MASTER
 *	Revised 7/19/17 - specific to msp430fr5738
 */

#include <msp430fr5738.h>
#include "SPI_MASTER.h"


//volatile unsigned char SPIRXBuffer[RX_BUFFER_SIZE];		// structure to hold received SPI data
//volatile unsigned char SPIRXBufferPointer = 0; 			// pointer to SPI data

static volatile unsigned char SPITXBuffer[TX_BUFFER_SIZE];		// structure to hold transmit SPI data (circular buffer)
static volatile unsigned char SPITXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char SPITXBufferEndPointer = 0;		// pointer to first empty slot in buffer

#pragma vector=USCI_B0_VECTOR		//SPI interrupt
__interrupt void SPI(void)
{
	switch(UCB0IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
//			SPIRXBuffer[SPIRXBufferPointer] = UCB0RXBUF;	// store character in string array
//			SPIRXBufferPointer++;		// increment pointer
		    SPIReceiveAction(UCB0RXBUF);
			break;

		case 4:		// TX interrupt
			UCB0TXBUF = SPITXBuffer[SPITXBufferSendoutPointer];
			SPITXBufferSendoutPointer++;
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
	UCB0CTLW0 |= UCCKPH + UCMSB + UCMST + UCSYNC + UCSSEL_2;	// MSB first, MasterMode, Sync
	UCB0BRW = 2u;
	UCB0CTLW0 &= ~UCSWRST;				// Bring out of reset
	UCB0IE |= UCRXIE;					// Enable TXinterrupt

	P1SEL1 |= BIT6 + BIT7;
	P2SEL1 |= BIT2;
}

void SPISendByte(char byte)
{
	SPITXBuffer[SPITXBufferEndPointer] = byte;
	SPITXBufferEndPointer++;
	if(SPITXBufferEndPointer >= TX_BUFFER_SIZE)
		SPITXBufferEndPointer = 0;
	UCB0IE |= UCTXIE;
}

void SPISendByteBlocking(char byte)
{
    UCB0IE &= ~UCTXIE;
    while(UCB0STATW & UCBUSY);
    UCB0TXBUF = byte;
}

char SPITransactionBlocking(char byte)
{
    UCB0IE &= ~(UCTXIE + UCRXIE);
    UCB0TXBUF = byte;
    while(UCB0STATW & UCBUSY);
    return UCB0RXBUF;
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

