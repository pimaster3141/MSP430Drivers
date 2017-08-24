/*
 * SPI_SLAVE.c (2.0)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *
 *	2.0	11/20/16	- Generic for MSP430X devices
 *					- UCSI instance defined by #define
 */

#include "deviceConfig.h"
#include "SPI_SLAVE.h"


#if SPI_SLAVE_INSTANCE == USCI_A0
#define SPI_SLAVE_USCI_IRQ USCI_A0_VECTOR
#define SPI_SLAVE_UC_IV UCA0IV
#define SPI_SLAVE_UC_RXBUF UCA0RXBUF
#define SPI_SLAVE_UC_TXBUF UCA0TXBUF
#define SPI_SLAVE_UC_IE UCA0IE
#define SPI_SLAVE_UC_CTL1 UCA0CTL1
#define SPI_SLAVE_UC_CTL0 UCA0CTL0
#define SPI_SLAVE_UC_STAT UCA0STAT

#elif SPI_SLAVE_INSTANCE == USCI_A1
#define SPI_SLAVE_USCI_IRQ USCI_A1_VECTOR
#define SPI_SLAVE_UC_IV UCA1IV
#define SPI_SLAVE_UC_RXBUF UCA1RXBUF
#define SPI_SLAVE_UC_TXBUF UCA1TXBUF
#define SPI_SLAVE_UC_IE UCA1IE
#define SPI_SLAVE_UC_CTL1 UCA1CTL1
#define SPI_SLAVE_UC_CTL0 UCA1CTL0
#define SPI_SLAVE_UC_STAT UCA1STAT

#elif SPI_SLAVE_INSTANCE == USCI_A2
#define SPI_SLAVE_USCI_IRQ USCI_A2_VECTOR
#define SPI_SLAVE_UC_IV UCA2IV
#define SPI_SLAVE_UC_RXBUF UCA2RXBUF
#define SPI_SLAVE_UC_TXBUF UCA2TXBUF
#define SPI_SLAVE_UC_IE UCA2IE
#define SPI_SLAVE_UC_CTL1 UCA2CTL1
#define SPI_SLAVE_UC_CTL0 UCA2CTL0
#define SPI_SLAVE_UC_STAT UCA2STAT

#elif SPI_SLAVE_INSTANCE == USCI_B0
#define SPI_SLAVE_USCI_IRQ USCI_B0_VECTOR
#define SPI_SLAVE_UC_IV UCB0IV
#define SPI_SLAVE_UC_RXBUF UCB0RXBUF
#define SPI_SLAVE_UC_TXBUF UCB0TXBUF
#define SPI_SLAVE_UC_IE UCB0IE
#define SPI_SLAVE_UC_CTL1 UCB0CTL1
#define SPI_SLAVE_UC_CTL0 UCB0CTL0
#define SPI_SLAVE_UC_STAT UCB0STAT

#elif SPI_SLAVE_INSTANCE == USCI_B1
#define SPI_SLAVE_USCI_IRQ USCI_B1_VECTOR
#define SPI_SLAVE_UC_IV UCB1IV
#define SPI_SLAVE_UC_RXBUF UCB1RXBUF
#define SPI_SLAVE_UC_TXBUF UCB1TXBUF
#define SPI_SLAVE_UC_IE UCB1IE
#define SPI_SLAVE_UC_CTL1 UCB1CTL1
#define SPI_SLAVE_UC_CTL0 UCB1CTL0
#define SPI_SLAVE_UC_STAT UCB1STAT

#elif SPI_SLAVE_INSTANCE == USCI_B2
#define SPI_SLAVE_USCI_IRQ USCI_B2_VECTOR
#define SPI_SLAVE_UC_IV UCB2IV
#define SPI_SLAVE_UC_RXBUF UCB2RXBUF
#define SPI_SLAVE_UC_TXBUF UCB2TXBUF
#define SPI_SLAVE_UC_IE UCB2IE
#define SPI_SLAVE_UC_CTL1 UCB2CTL1
#define SPI_SLAVE_UC_CTL0 UCB2CTL0
#define SPI_SLAVE_UC_STAT UCB2STAT
#endif

#if SPI_SLAVE_CLK_PORT == PORT1
#define SPI_SLAVE_CLK_P_DIR P1DIR
#define SPI_SLAVE_CLK_P_SEL P1SEL

#elif SPI_SLAVE_CLK_PORT == PORT2
#define SPI_SLAVE_CLK_P_DIR P2DIR
#define SPI_SLAVE_CLK_P_SEL P2SEL

#elif SPI_SLAVE_CLK_PORT == PORT3
#define SPI_SLAVE_CLK_P_DIR P3DIR
#define SPI_SLAVE_CLK_P_SEL P3SEL

#elif SPI_SLAVE_CLK_PORT == PORT4
#define SPI_SLAVE_CLK_P_DIR P4DIR
#define SPI_SLAVE_CLK_P_SEL P4SEL
#endif

#if SPI_SLAVE_SOMI_PORT == PORT1
#define SPI_SLAVE_SOMI_P_DIR P1DIR
#define SPI_SLAVE_SOMI_P_SEL P1SEL

#elif SPI_SLAVE_SOMI_PORT == PORT2
#define SPI_SLAVE_SOMI_P_DIR P2DIR
#define SPI_SLAVE_SOMI_P_SEL P2SEL

#elif SPI_SLAVE_SOMI_PORT == PORT3
#define SPI_SLAVE_SOMI_P_DIR P3DIR
#define SPI_SLAVE_SOMI_P_SEL P3SEL

#elif SPI_SLAVE_SOMI_PORT == PORT4
#define SPI_SLAVE_SOMI_P_DIR P4DIR
#define SPI_SLAVE_SOMI_P_SEL P4SEL
#endif

#if SPI_SLAVE_SIMO_PORT == PORT1
#define SPI_SLAVE_SIMO_P_DIR P1DIR
#define SPI_SLAVE_SIMO_P_SEL P1SEL

#elif SPI_SLAVE_SIMO_PORT == PORT2
#define SPI_SLAVE_SIMO_P_DIR P2DIR
#define SPI_SLAVE_SIMO_P_SEL P2SEL

#elif SPI_SLAVE_SIMO_PORT == PORT3
#define SPI_SLAVE_SIMO_P_DIR P3DIR
#define SPI_SLAVE_SIMO_P_SEL P3SEL

#elif SPI_SLAVE_SIMO_PORT == PORT4
#define SPI_SLAVE_SIMO_P_DIR P4DIR
#define SPI_SLAVE_SIMO_P_SEL P4SEL
#endif



// volatile unsigned char SPIRXBuffer[RX_BUFFER_SIZE];		// structure to hold received SPI data
// volatile unsigned char SPIRXBufferPointer = 0; 			// pointer to SPI data

static volatile unsigned char SPITXBuffer[TX_BUFFER_SIZE];		// structure to hold transmit SPI data (circular buffer)
static volatile unsigned char SPITXBufferSendoutPointer = 0;	// pointer to next char to be sent out
static volatile unsigned char SPITXBufferEndPointer = 0;		// pointer to first empty slot in buffer

#pragma vector=SPI_SLAVE_USCI_IRQ		//SPI interrupt
__interrupt void SPI_ISR(void)
{
	switch(SPI_SLAVE_UC_IV)
	{
		case 0:		// no interrupt
			break;

		case 2:		// RX interupt
			// SPIRXBuffer[SPIRXBufferPointer] = SPI_SLAVE_UC_RXBUF;	// store character in string array
			// SPIRXBufferPointer++;		// increment pointer
			SPIReceiveAction(SPI_SLAVE_UC_RXBUF);
			break;

		case 4:		// TX interrupt
			SPI_SLAVE_UC_TXBUF = SPITXBuffer[SPITXBufferSendoutPointer];
			SPITXBufferSendoutPointer++;
//			SPITXBufferSendoutPointer = (SPITXBufferSendoutPointer+1) % TXBufferSize;
			if(SPITXBufferSendoutPointer >= TX_BUFFER_SIZE)
				SPITXBufferSendoutPointer = 0;
			if(SPITXBufferSendoutPointer == SPITXBufferEndPointer)
				SPI_SLAVE_UC_IE &= ~UCTXIE;
			break;

		default:	// WTF...
			break;
	}
}

void SPISetup(void)
{
	SPI_SLAVE_UC_CTL1 |= UCSWRST;				// Hold SPI
	SPI_SLAVE_UC_CTL0 |= UCCKPH + UCMSB + UCMODE_2 + UCSYNC;	// MSB first, MasterMode, Sync
	SPI_SLAVE_UC_IE |= UCRXIE;					// Enable TXinterrupt

	SPI_SLAVE_CLK_P_DIR &= ~(SPI_SLAVE_CLK_PIN);
	SPI_SLAVE_SIMO_P_DIR &= ~(SPI_SLAVE_SIMO_PIN);
	SPI_SLAVE_SOMI_P_DIR |= SPI_SLAVE_SOMI_PIN;

	SPI_SLAVE_CLK_P_SEL |= SPI_SLAVE_CLK_PIN;
	SPI_SLAVE_SIMO_P_SEL |= SPI_SLAVE_SIMO_PIN;
	SPI_SLAVE_SOMI_P_SEL |= SPI_SLAVE_SOMI_PIN;

	// P4DIR &= ~(BIT1 + BIT3);			// Set pin for input direction
	// P4SEL |= BIT1 + BIT2 + BIT3; 		// Set pins for secondary functions (SPI)

	SPI_SLAVE_UC_CTL1 &= ~UCSWRST;				// Bring out of reset

	SPI_SLAVE_UC_IE |= UCRXIE;					// Enable RX interrupts
}

void SPISendByte(char byte)
{
	SPITXBuffer[SPITXBufferEndPointer] = byte;
	SPITXBufferEndPointer++;
	if(SPITXBufferEndPointer >= TX_BUFFER_SIZE)
		SPITXBufferEndPointer = 0;
	SPI_SLAVE_UC_IE |= UCTXIE;
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
	SPI_SLAVE_UC_IE |= UCTXIE;
	return;
}

// void clearSPIRXBuffer(void)
// {
// 	while(SPI_SLAVE_UC_STAT & UCBUSY);
// 	SPIRXBufferPointer = 0;
// }

// void readSPIRXBuffer(char *outputBuffer, char length)
// {
// 	char i;
// 	for (i = 0; i < length; i++)
// 		outputBuffer[i] = SPIRXBuffer[i];
// 	return;
// }

