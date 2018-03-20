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
 *	Revised 8/31/16 - Update for F5172 Ports
 */

#include <msp430f5172.h>
#include "UART.h"


// static volatile unsigned char UARTRXBuffer[UART_RX_BUFFER_SIZE];		// structure to hold received UART data
// static volatile unsigned char UARTRXBufferPointer = 0; 			// pointer to UART data
// static volatile unsigned char UARTRXBufferEndPointer = 0;

static volatile unsigned char UARTTXBuffer[UART_TX_BUFFER_SIZE];		// structure to hold transmit UART data (circular buffer)
static volatile unsigned char UARTTXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char UARTTXBufferEndPointer = 0;		// pointer to first empty slot in buffer



#pragma vector=USCI_A0_VECTOR		//UART interrupt
__interrupt void UART_ISR(void)
{
	switch(UCA0IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// UARTRXBuffer[UARTRXBufferPointer] = UCA0RXBUF;	// store character in string array
			// UARTRXBufferPointer++;		// increment pointer
			UARTReceiveAction(UCA0RXBUF);
			break;
		case 4:		// TX interrupt
			UCA0TXBUF = UARTTXBuffer[UARTTXBufferSendoutPointer];
//			UARTTXBufferSendoutPointer = (UARTTXBufferSendoutPointer+1) % TXBufferSize;
			UARTTXBufferSendoutPointer++;
			if(UARTTXBufferSendoutPointer == UART_TX_BUFFER_SIZE)
				UARTTXBufferSendoutPointer = 0;
			if(UARTTXBufferSendoutPointer == UARTTXBufferEndPointer)
				UCA0IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}



void UARTSetup(void)
{
	// Setup UART, 8N1 460800 baud, even
	UCA0CTL1 |= UCSSEL_2 + UCSWRST;	// CLK = SMCLK(DCO)
	UCA0BR0 = 68u;					// SET BAUD RATE
	UCA0BR1 = 0;					// N = clk/baud

	P1DIR &= ~(BIT1 + BIT2);		// Set pin for input direction
	P1SEL |= BIT1 + BIT2; 			// Set pins for secondary functions (UART)
	UCA0CTL1 &= ~UCSWRST;			// init uart

	__bis_SR_register(GIE);			//enter LPMx
//	__delay_cycles(60000);
//	__delay_cycles(60000);
	UCA0IE |= UCRXIE;				// Enable interrupts
}

void UARTSendByte(char byte)
{
	UARTTXBuffer[UARTTXBufferEndPointer] = byte;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;
	UCA0IE |= UCTXIE;
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
	UCA0IE |= UCTXIE;
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
	UCA0IE |= UCTXIE;
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
