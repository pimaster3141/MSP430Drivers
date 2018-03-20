void I2CSetup(char prescale)
{
	UCB1CTL1 |= UCSWRST;
	UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;
	UCB1BR0 = prescale;
	UCB1BR1 = 0;
}