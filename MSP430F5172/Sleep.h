/*
 * Sleep.h
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 *		
 *	Revised 8/31/16 - Update for F5172 Ports
 */


#ifndef SLEEP_H_
#define SLEEP_H_

// suspends the processor for the specified number of microseconds
void sleep(unsigned int usec);

// suspends the processor for the spcified number of miliseconds
void longSleep(unsigned int msec);

#endif /* SLEEP_H_ */
