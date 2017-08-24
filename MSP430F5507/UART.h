/*
 * UART.h (1.1)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	Revised 6/15/16 - Added generic receive action. 
 *	Revised 6/17/16 - Removed Recieve Buffers.
 *					- Renamed Methods
 */

#ifndef UART_H_
#define UART_H_

#define UART_TX_BUFFER_SIZE 50u
// #define UART_RX_BUFFER_SIZE 50u


void UARTSetup(void);
void UARTSendByte(char byte);
void UARTSendArray(char *TxArray, char ArrayLengh);
void UARTSendCRLF(void);
// void clearUARTRXBuffer(void);
void UARTSendIntArray(int *data, char length);
void UARTReceiveAction(unsigned char byte);

#endif /* UART_H_ */
