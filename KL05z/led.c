//by Małgorzata Kosakowska and Michał Ptak
#include "led.h"

void Zumo_Led_Init(void)
{
	PORTB->PCR[0] |= PORT_PCR_MUX(1); //diode port ALT1
	PTB->PDDR |= (1<<0); //ptb0 as output
	PTB->PDOR |= (1<<0); //turn off diode
}


void LED(int a)
{
	if(a==1) PTB->PDOR |= (1<<0);	//lights and turns off diode
		else	PTB->PDOR &= ~(1<<0);
}
