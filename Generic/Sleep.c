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

#include "Sleep.h"
#include "deviceConfig.h"

#if SLEEP_TIMER_INSTANCE == TIMER_A0
#define SLP_TA_CTL TA0CTL
#define SLP_TA_CCTL0 TA0CCTL0
#define SLP_TA_CCR0 TA0CCR0
#define SLP_TIMER_IRQ TIMER0_A0
#define SLP_TIMER_IRQ_VECTOR TIMER0_A0_VECTOR

#elif SLEEP_TIMER_INSTANCE == TIMER_A1
#define SLP_TA_CTL TA1CTL
#define SLP_TA_CCTL0 TA1CCTL0
#define SLP_TA_CCR0 TA1CCR0
#define SLP_TIMER_IRQ TIMER1_A0
#define SLP_TIMER_IRQ_VECTOR TIMER1_A0_VECTOR

#elif SLEEP_TIMER_INSTANCE == TIMER_A2
#define SLP_TA_CTL TA2CTL
#define SLP_TA_CCTL0 TA2CCTL0
#define SLP_TA_CCR0 TA2CCR0
#define SLP_TIMER_IRQ TIMER2_A0
#define SLP_TIMER_IRQ_VECTOR TIMER2_A0_VECTOR
#endif


static volatile unsigned int sleepLoopCounter = 0;				//sleep cycle counter for sleep function


/*
 * Sleep function to pause execution without sofware loops
 * int usec assumes 5MHZ SMCLK, powermode = LPMx_bits
 * usec must be < ~13ms, use longSleep instead
 */
void sleep(unsigned int usec, unsigned int powerMode)
{
	SLP_TA_CTL |= TASSEL_2 + ID_2 + MC_1 + TACLR;	//Timer2 wtih SMCLK, continuous
	SLP_TA_CCTL0 |= CCIE;					//Timer2 compare interrupt enable
	SLP_TA_CCR0 = usec * 5;					//Timer2 compare value
	sleepLoopCounter = 0;				//no timer resets
	_bis_SR_register(powerMode + GIE);	//enter LPMx
	return;
}

/*
 * Sleep function to pause execution without sofware loops
 * int msec assumes 5MHZ SMCLK, powermode = LPMx_bits
 * msec must be 16bit
 */
void longSleep(unsigned int msec, unsigned int powerMode)
{
	SLP_TA_CTL |= TASSEL_2 + ID_2 + MC_1 + TACLR;	//Timer2 wtih SMCLK, continuous
	SLP_TA_CCTL0 |= CCIE;					//Timer2 compare interrupt enable
	SLP_TA_CCR0 = 5000;					 	//Timer2 compare value (1ms)
	sleepLoopCounter = msec-1;
	_bis_SR_register(powerMode + GIE);	//enter LPMx
	return;
}

#pragma vector = SLP_TIMER_IRQ_VECTOR	//Timer2 A0 interrupt
__interrupt void SLP_TIMER_IRQ(void)	// sleep timer resume to AM
{
	if(sleepLoopCounter > 0) 		//decrement loop counter for longer sleeps
	{
		sleepLoopCounter--;
	}
	else
	{
		SLP_TA_CTL = 0;					//halt timer
		_bic_SR_register_on_exit(LPM4_bits);	//return to AM
	}
	return;
}
