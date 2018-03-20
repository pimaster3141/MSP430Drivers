/*
 * UART.c (2.0)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	1.1 6/15/16 	- Added generic receive action.
 *	1.2 6/16/16 	- changed assuming 15.73MHz SMCLK
 *	1.3 6/17/16 	- Removed Receive Buffers.
 *	1.4 6/18/16 	- Added Pause in Init for stabilization
 *	2.0	11/20/16	- Generic for MSP430X devices
 *					- UCSI instance defined by #define
 */

#include "deviceConfig.h"
#include "UART.h"

#if UART_INSTANCE == USCI_A0
#define UART_USCI_IRQ USCI_A0_VECTOR
#define UART_UC_IV UCA0IV
#define UART_UC_RXBUF UCA0RXBUF
#define UART_UC_TXBUF UCA0TXBUF
#define UART_UC_IE UCA0IE
#define UART_UC_CTL1 UCA0CTL1
#define UART_UC_CTL0 UCA0CTL0
#define UART_UC_STAT UCA0STAT
#define UART_UC_BR0	UCA0BR0
#define UART_UC_BR1 UCA0BR1

#elif UART_INSTANCE == USCI_A1
#define UART_USCI_IRQ USCI_A1_VECTOR
#define UART_UC_IV UCA1IV
#define UART_UC_RXBUF UCA1RXBUF
#define UART_UC_TXBUF UCA1TXBUF
#define UART_UC_IE UCA1IE
#define UART_UC_CTL1 UCA1CTL1
#define UART_UC_CTL0 UCA1CTL0
#define UART_UC_STAT UCA1STAT
#define UART_UC_BR0	UCA1BR0
#define UART_UC_BR1 UCA1BR1

#elif UART_INSTANCE == USCI_A2
#define UART_USCI_IRQ USCI_A2_VECTOR
#define UART_UC_IV UCA2IV
#define UART_UC_RXBUF UCA2RXBUF
#define UART_UC_TXBUF UCA2TXBUF
#define UART_UC_IE UCA2IE
#define UART_UC_CTL1 UCA2CTL1
#define UART_UC_CTL0 UCA2CTL0
#define UART_UC_STAT UCA2STAT
#define UART_UC_BR0	UCA2BR0
#define UART_UC_BR1 UCA2BR1
#endif


#if UART_TX_PORT == PORT1
#define UART_TX_P_DIR P1DIR
#define UART_TX_P_SEL P1SEL

#elif UART_TX_PORT == PORT2
#define UART_TX_P_DIR P2DIR
#define UART_TX_P_SEL P2SEL

#elif UART_TX_PORT == PORT3
#define UART_TX_P_DIR P3DIR
#define UART_TX_P_SEL P3SEL

#elif UART_TX_PORT == PORT4
#define UART_TX_P_DIR P4DIR
#define UART_TX_P_SEL P4SEL
#endif

#if UART_RX_PORT == PORT1
#define UART_RX_P_DIR P1DIR
#define UART_RX_P_SEL P1SEL

#elif UART_RX_PORT == PORT2
#define UART_RX_P_DIR P2DIR
#define UART_RX_P_SEL P2SEL

#elif UART_RX_PORT == PORT3
#define UART_RX_P_DIR P3DIR
#define UART_RX_P_SEL P3SEL

#elif UART_RX_PORT == PORT4
#define UART_RX_P_DIR P4DIR
#define UART_RX_P_SEL P4SEL
#endif

// #if UART_SIMO_PORT == PORT1
// #define UART_SIMO_P_DIR P1DIR
// #define UART_SIMO_P_SEL P1SEL

// #elif UART_SIMO_PORT == PORT2
// #define UART_SIMO_P_DIR P2DIR
// #define UART_SIMO_P_SEL P2SEL

// #elif UART_SIMO_PORT == PORT3
// #define UART_SIMO_P_DIR P3DIR
// #define UART_SIMO_P_SEL P3SEL

// #elif UART_SIMO_PORT == PORT4
// #define UART_SIMO_P_DIR P4DIR
// #define UART_SIMO_P_SEL P4SEL
// #endif

// static volatile unsigned char UARTRXBuffer[UART_RX_BUFFER_SIZE];		// structure to hold received UART data
// static volatile unsigned char UARTRXBufferPointer = 0; 			// pointer to UART data
// static volatile unsigned char UARTRXBufferEndPointer = 0;

static volatile unsigned char UARTTXBuffer[UART_TX_BUFFER_SIZE];		// structure to hold transmit UART data (circular buffer)
static volatile unsigned char UARTTXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char UARTTXBufferEndPointer = 0;		// pointer to first empty slot in buffer



#pragma vector=UART_USCI_IRQ		//UART interrupt
__interrupt void UART_ISR(void)
{
	switch(UART_UC_IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// UARTRXBuffer[UARTRXBufferPointer] = UART_UC_RXBUF;	// store character in string array
			// UARTRXBufferPointer++;		// increment pointer
			UARTReceiveAction(UART_UC_RXBUF);
			break;
		case 4:		// TX interrupt
			UART_UC_TXBUF = UARTTXBuffer[UARTTXBufferSendoutPointer];
//			UARTTXBufferSendoutPointer = (UARTTXBufferSendoutPointer+1) % TXBufferSize;
			UARTTXBufferSendoutPointer++;
			if(UARTTXBufferSendoutPointer == UART_TX_BUFFER_SIZE)
				UARTTXBufferSendoutPointer = 0;
			if(UARTTXBufferSendoutPointer == UARTTXBufferEndPointer)
				UART_UC_IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}



void UARTSetup(void)
{
	// Setup UART, 8N1 460800 baud, even
	UART_UC_CTL1 |= UCSSEL_2 + UCSWRST;	// CLK = SMCLK(DCO)
	UART_UC_BR0 = UART_BR0;					// SET BAUD RATE
	UART_UC_BR1 = UART_BR1;					// N = clk/baud

	UART_TX_P_DIR |= UART_TX_PIN;
	UART_RX_P_DIR &= ~(UART_RX_PIN);

	UART_TX_P_SEL |= UART_TX_PIN;
	UART_RX_P_SEL |= UART_RX_PIN;

	// P4DIR &= ~(BIT4 + BIT5);		// Set pin for input direction
	// P4SEL |= BIT4 + BIT5; 			// Set pins for secondary functions (UART)
	UART_UC_CTL1 &= ~UCSWRST;			// init uart

	__bis_SR_register(GIE);			//enter LPMx
//	__delay_cycles(60000);
//	__delay_cycles(60000);
	UART_UC_IE |= UCRXIE;				// Enable interrupts
}

void UARTSendByte(char byte)
{
	UARTTXBuffer[UARTTXBufferEndPointer] = byte;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;
	UART_UC_IE |= UCTXIE;
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
	UART_UC_IE |= UCTXIE;
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
	UART_UC_IE |= UCTXIE;
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
