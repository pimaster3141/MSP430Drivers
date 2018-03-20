/*
 * deviceConfig.h (1.0)
 *
 *  Created on: Nov 20, 2016
 *      Author: jason
 */

#ifndef DEVICECONFIG_H_
#define DEVICECONFIG_H_

// Define Processor Variant
#define PROCESSOR MSP430FR5738


// Define MCLK parameters
#define CPUFREQ 20000000
#define UCS_DCORSEL DCORSEL_5


// Define TimerA instance for Sleep
#define SLEEP_TIMER_INSTANCE TIMER_A0


//Define USCI Instance for SPI_SLAVE
#define SPI_SLAVE_INSTANCE USCI_B0

#define SPI_SLAVE_CLK_PORT PORT2
#define SPI_SLAVE_CLK_PIN BIT2

#define SPI_SLAVE_SIMO_PORT PORT1
#define SPI_SLAVE_SIMO_PIN BIT6

#define SPI_SLAVE_SOMI_PORT PORT1
#define SPI_SLAVE_SOMI_PIN BIT7


// Define USCI Instance for UART
#define UART_BR0 174u
#define UART_BR1 0u
#define UART_INSTANCE USCI_A0

#define UART_TX_PORT PORT1
#define UART_TX_PIN BIT0

#define UART_RX_PORT PORT1
#define UART_RX_PIN BIT1

	//optional defines (do not define if not needed)
#define UART_CTS_PORT PORT1
#define UART_CTS_BIT BIT0

#define UART_RTS_PORT PORT1
#define UART_RTS_PIN BIT0



// Computed Identifiers (do not modify)
#define CALTDH0CTL1_200 *((unsigned int *)0x1A34)

#define MSP430FR5738 0

#define TIMER_A0 0
#define TIMER_A1 1
#define TIMER_A2 2
#define TIMER_A3 3

#define USCI_A0 0
#define USCI_A1 1
#define USCI_A2 2
#define USCI_B0 3
#define USCI_B1 4
#define USCI_B2 5

#define PORTJ 0
#define PORT1 1
#define PORT2 2
#define PORT3 3
#define PORT4 4
#define PORT5 5


#if PROCESSOR == MSP430FR5738
#include <msp430fr5738.h>
#endif


#endif /* DEVICECONFIG_H_ */
