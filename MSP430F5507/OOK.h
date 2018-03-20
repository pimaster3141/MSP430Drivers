/*
 * OOK.h (1.0)
 *
 *  Created on: Jun 20, 2016
 *      Author: jasony
 */



#define OOKPORT P1OUT
#define OOKBIT BIT2
#define OOKDIR P1DIR

#ifndef OOK_H_
#define OOK_H_

#ifndef OOKPORT
#error Need to define OOKPORT e.g - P1OUT
#endif

#ifndef OOKBIT
#error Need to define OOKBIT e.g - BIT0
#endif

#ifndef OOKDIR
#error Need to define OOKDIR e.g - P1DIR
#endif

void OOKSetup(char lowTime, char highTime);
void OOKSendByte(char byte);
void OOKSendArray(char *TxArray, unsigned char ArrayLength);

#endif /* OOK_H_ */
