/*
 * UART.c (1.1)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	Revised 6/15/16 - Added generic receive action.
 *	Revised 6/16/16 - changed assuming 15.73MHz SMCLK
 *	Revised 6/17/16 - Removed Receive Buffers.
 *	Revised 6/18/16 - Added Pause in Init for stabilization
 */

#include <msp430f5507.h>
#include "UART.h"


// static volatile unsigned char UARTRXBuffer[UART_RX_BUFFER_SIZE];		// structure to hold received UART data
// static volatile unsigned char UARTRXBufferPointer = 0; 			// pointer to UART data
// static volatile unsigned char UARTRXBufferEndPointer = 0;

static volatile unsigned char UARTTXBuffer[UART_TX_BUFFER_SIZE];		// structure to hold transmit UART data (circular buffer)
static volatile unsigned char UARTTXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char UARTTXBufferEndPointer = 0;		// pointer to first empty slot in buffer



#pragma vector=USCI_A1_VECTOR		//UART interrupt
__interrupt void UART_ISR(void)
{
	switch(UCA1IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// UARTRXBuffer[UARTRXBufferPointer] = UCA1RXBUF;	// store character in string array
			// UARTRXBufferPointer++;		// increment pointer
			UARTReceiveAction(UCA1RXBUF);
			break;
		case 4:		// TX interrupt
			UCA1TXBUF = UARTTXBuffer[UARTTXBufferSendoutPointer];
//			UARTTXBufferSendoutPointer = (UARTTXBufferSendoutPointer+1) % TXBufferSize;
			UARTTXBufferSendoutPointer++;
			if(UARTTXBufferSendoutPointer == UART_TX_BUFFER_SIZE)
				UARTTXBufferSendoutPointer = 0;
			if(UARTTXBufferSendoutPointer == UARTTXBufferEndPointer)
				UCA1IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}



void UARTSetup(void)
{
	// Setup UART, 8N1 460800 baud, even
	UCA1CTL1 |= UCSSEL_2 + UCSWRST;	// CLK = SMCLK(DCO)
	UCA1BR0 = 34u;					// SET BAUD RATE
	UCA1BR1 = 0;					// N = clk/baud

	P4DIR &= ~(BIT4 + BIT5);		// Set pin for input direction
	P4SEL |= BIT4 + BIT5; 			// Set pins for secondary functions (UART)
	UCA1CTL1 &= ~UCSWRST;			// init uart

	__bis_SR_register(GIE);			//enter LPMx
//	__delay_cycles(60000);
//	__delay_cycles(60000);
	UCA1IE |= UCRXIE;				// Enable interrupts
}

void UARTSendByte(char byte)
{
	UARTTXBuffer[UARTTXBufferEndPointer] = byte;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;
	UCA1IE |= UCTXIE;
}

void UARTSendArray(char *TxArray, char ArrayLength)
{
	while(ArrayLength--)
	{
		UARTTXBuffer[UARTTXBufferEndPointer] = *TxArray;
		TxArray++;
		UARTTXBufferEndPointer++;
		if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
			UARTTXBufferEndPointer = 0;
	}
	UCA1IE |= UCTXIE;
	return;
}

void UARTSendCRLF(void)
{
	UARTTXBuffer[UARTTXBufferEndPointer] = 0x0D;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;

	UARTTXBuffer[UARTTXBufferEndPointer] = 0x0A;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;
	UCA1IE |= UCTXIE;
	return;
}

// void clearUARTRXBuffer(void)
// {
// 	UARTRXBufferPointer = 0;
// }

void UARTSendIntArray(int *data, char length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		int current = data[i];
		char lowbyte = (char) (current & 0x00FF);
		char highbyte = (char) ((current >> 8) & 0x00FF);
		UARTSendByte(highbyte);
		UARTSendByte(lowbyte);
	}
	return;
}
