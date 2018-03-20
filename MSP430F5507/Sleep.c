/*
 * Sleep.c
 *
 *  Created on: Aug 7, 2015
 *      Author: jasoyang
 */
#include <msp430f5507.h>
#include "Sleep.h"


static volatile unsigned int sleepLoopCounter = 0;				//sleep cycle counter for sleep function


/*
 * Sleep function to pause execution without sofware loops
 * int usec assumes 5MHZ SMCLK, powermode = LPMx_bits
 * usec must be < ~13ms, use longSleep instead
 */
void sleep(unsigned int usec)
{
	TA2CTL |= TASSEL_2 + ID_2 + MC_1 + TACLR;	//Timer2 wtih SMCLK, continuous
	TA2CCTL0 |= CCIE;					//Timer2 compare interrupt enable
	TA2CCR0 = usec * 5;					//Timer2 compare value
	sleepLoopCounter = 0;				//no timer resets
	_bis_SR_register(LPM0_bits + GIE);	//enter LPMx
	return;
}

/*
 * Sleep function to pause execution without sofware loops
 * int msec assumes 5MHZ SMCLK, powermode = LPMx_bits
 * msec must be 16bit
 */
void longSleep(unsigned int msec)
{
	TA2CTL |= TASSEL_2 + ID_2 + MC_1 + TACLR;	//Timer2 wtih SMCLK, continuous
	TA2CCTL0 |= CCIE;					//Timer2 compare interrupt enable
	TA2CCR0 = 5000;					 	//Timer2 compare value (1ms)
	sleepLoopCounter = msec-1;
	_bis_SR_register(LPM0_bits + GIE);	//enter LPMx
	return;
}

#pragma vector = TIMER2_A0_VECTOR	//Timer2 A0 interrupt
__interrupt void Timer2_A0(void)	// sleep timer resume to AM
{
	if(sleepLoopCounter > 0) 		//decrement loop counter for longer sleeps
	{
		sleepLoopCounter--;
	}
	else
	{
		TA2CTL = 0;					//halt timer
		_bic_SR_register_on_exit(LPM4_bits);	//return to AM
	}
	return;
}
