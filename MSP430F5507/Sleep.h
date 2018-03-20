/*
 * Sleep.h
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 */


#ifndef PRIMUS_SLEEP_H_
#define PRIMUS_SLEEP_H_

// suspends the processor for the specified number of microseconds
void sleep(unsigned int usec);

// suspends the processor for the spcified number of miliseconds
void longSleep(unsigned int msec);

#endif /* PRIMUS_SLEEP_H_ */
