/*
 * UCS.c (1.0.1)
 *
 *  Created on: Nov 21, 2016
 *      Author: jason
 *
 *	1.0.1 	11/27/16	- Ported to MSP430FR5738 SPECIFIC - NON GENERIC
 */

#include "deviceConfig.h"
#include "UCS.h"

#define _dcoMult (((CPUFREQ*10/32768)+5)/10-1)

void setDCO(void)
{
	CSCTL0 = CSKEY;	// unlock clock

	CSCTL1 = DCOFSEL_1;	// Set for 6.67MHz
	CSCTL2 = SELA_3 + SELS_3 + SELM_3; 	// Set ACLK, SMCLK, MCLK = DCO
	CSCTL3 = DIVA_5;	// Div Aclk/32
	CSCTL4 = XT1OFF + XT2OFF;	//Disable external crystals

//	CSCTL0 = 0x00;		// lock clock

//	UCSCTL3 = SELREF__REFOCLK;		// select REFO as FLL source
//	UCSCTL6 = XT1OFF | XT2OFF;    	// turn off XT1 and XT2
//
//	_bis_SR_register(SCG0);      	// Disable the FLL control loop
//	UCSCTL0 = 0x0000u;              // Set lowest possible DCOx, MODx
//	UCSCTL1 = UCS_DCORSEL;    	  	// Set RSELx for DCO = 20 MHz -- LOOKUP
//	UCSCTL2 = _dcoMult;        			// Set DCO Multiplier
//										// (N + 1) * FLLRef = Fdco
//										// (304 + 1) * 32768 = 5MHz
//
//	UCSCTL4 = SELA__DCOCLK + SELS__DCOCLK + SELM__DCOCLK;	// Set Clock Sources
//																// ACLK = DCO
//																// SMCLK = DCO
//																// MCLK = DCO
//
//	_bic_SR_register(SCG0);  	  	// Enable the FLL control loop
	__delay_cycles(60000);
	__delay_cycles(60000);
	return;
}


