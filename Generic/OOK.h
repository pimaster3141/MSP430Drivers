/*
 * OOK.c (2.0)
 *
 *  Created on: Jun 17, 2016
 *      Author: jasony
 *
 *	v1.1 8/31/16 - Update for F5172 Ports
 *	v2.0 11/20/16 - Added prototypes to be defined in main
 *				- Added powermode def
 *
 */



#ifndef OOK_H_
#define OOK_H_

void OOKSetup(char lowTime, char highTime, unsigned int powerMode);
void OOKSendByte(char byte);
void OOKSendArray(char *TxArray, unsigned char ArrayLength);
void OOKHighAction();
void OOKLowAction();

#endif /* OOK_H_ */
