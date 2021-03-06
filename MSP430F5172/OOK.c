/*
 * OOK.c (1.1)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *		
 *	Revised 8/31/16 - Update for F5172 Ports
 *
 */

#include <msp430F5172.h>
#include "OOK.h"
#include "sleep.h"


static char OOKLow;
static char OOKHigh;

void OOKSetup(char lowTime, char highTime)
{
	OOKDIR |= OOKBIT;
	OOKPORT |= OOKBIT;

	OOKLow = lowTime;
	OOKHigh = highTime;
	return;
}

void OOKSendByte(char byte)
{
	//for every bit in byte
	char bit = 0;
	for(bit = 0; bit < 8; bit++)
	{
		//see if first bit is 1
		if((byte & 0x80) == 0x80)
		{
			OOKPORT |= OOKBIT;
			sleep(OOKLow);
			OOKPORT &= ~OOKBIT;
			sleep(OOKHigh);
		}
		// first bit is 0
		else
		{
			OOKPORT |= OOKBIT;
			sleep(OOKHigh);
			OOKPORT &= ~OOKBIT;
			sleep(OOKLow);
		}
		// left rotate for mask
		byte = byte << 1;
	}
}

void OOKSendArray(char *TxArray, unsigned char ArrayLength)
{
	OOKPORT |= OOKBIT;
	sleep(OOKLow);

	while(ArrayLength--)
	{
		OOKSendByte(*TxArray);
		TxArray++;
	}

	OOKPORT |= OOKBIT;
	return;
}
