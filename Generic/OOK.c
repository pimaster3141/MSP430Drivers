/*
 * OOK.c (2.0)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *
 *	v1.1 8/31/16 - Update for F5172 Ports
 *	v2.0 11/20/16 - Added prototypes to be defined in main
 *					Added Powermode def
 *
 */

#include "OOK.h"

#include <deviceConfig.H>

#include "sleep.h"



static char OOKLow;
static char OOKHigh;
static unsigned int powerMode_;

void OOKSetup(char lowTime, char highTime, unsigned int powerMode)
{
	OOKLow = lowTime;
	OOKHigh = highTime;
	powerMode_ = powerMode;

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
			OOKHighAction();
			sleep(OOKLow, powerMode_);
			OOKLowAction();
			sleep(OOKHigh, powerMode_);
		}
		// first bit is 0
		else
		{
			OOKHighAction();
			sleep(OOKHigh, powerMode_);
			OOKLowAction();
			sleep(OOKLow, powerMode_);
		}
		// left rotate for mask
		byte = byte << 1;
	}
	OOKHighAction();
}

void OOKSendArray(char *TxArray, unsigned char ArrayLength)
{
	while(ArrayLength--)
	{
		OOKSendByte(*TxArray);
		TxArray++;
	}
	return;
}
