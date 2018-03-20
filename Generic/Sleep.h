/*
 * Sleep.c (2.0)
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *
 *	1.1 8/31/16 	- Update for F5172 Ports
 *	2.0	11/20/16	- Generic for MSP430X devices
 *					- Timer instance defined by #define
 *					- Added powermode option
 *
 */

#ifndef SLEEP_H_
#define SLEEP_H_

// suspends the processor for the specified number of microseconds
void sleep(unsigned int usec, unsigned int powerMode);

// suspends the processor for the specified number of milliseconds
void longSleep(unsigned int msec, unsigned int powerMode);

#endif /* SLEEP_H_ */
