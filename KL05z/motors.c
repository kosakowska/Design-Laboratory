//by Małgorzata Kosakowska and Michał Ptak

#include "motors.h"
#include <stdlib.h>

#define MOD_100Hz	6554		// MOD for 100Hz freq

void Motor_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      // Turn on port B
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;				//Turn on port A

	// Choosing role for pin port B ALT2
	PORTB->PCR[11] |= PORT_PCR_MUX(2);		// right caterpillar 	TPM0_CH3
	PORTA->PCR[5] |= PORT_PCR_MUX(2); 	//left caterpillar
	
	//Choosing role for pin port ALT1
	PORTB->PCR[7] |=PORT_PCR_MUX(1);
	PORTB->PCR[10] |=PORT_PCR_MUX(1);
	
	//Role as output
	PTB->PDDR |= (1<<11);	
	PTA->PDDR |= (1<<5);
	//Role as output (caterpillar)
	PTB->PDDR |= (1<<7)|(1<<10);
	
	///////////config counter TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;				//Adding clock signal to TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);			  // Choose source of clock TPMx MCGFLLCLK=41943040Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;					//	TPM0 in forward counting mode
	TPM0->SC |= TPM_SC_PS(6);								//	Input clock divider = 64; clock =655360Hz
	TPM0->MOD = MOD_100Hz;													//	MODULO=6553 - output freq=100Hz
	
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;	//	TPM0, kanal 1 (PTB11) "Edge-aligned PWM Low-true pulses (set Output on match, clear Output on reload)"
	TPM0->CONTROLS[0].CnV = 0x3334;					// Initial fulfillment factor = full
	
	TPM0->CONTROLS[5].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;	//	TPM0, kanal 5 (PTA5) "Edge-aligned PWM Low-true pulses (set Output on match, clear Output on reload)"
	TPM0->CONTROLS[5].CnV = 0x3334;					
	
	TPM0->SC |= TPM_SC_CMOD(1);							// turn on TPM0 counter
}

void set_motor_R(int k_curr){ //k-curr the bigger the faster moves
	if(k_curr>=0) PTB->PDOR &= ~(1<<7); //direction
		else PTB->PDOR |=(1<<7);
	
	uint16_t	ampl;
	uint16_t	mod_curr=MOD_100Hz;
	ampl=((int)mod_curr*(100-abs(k_curr)))/100;
	TPM0->CONTROLS[0].CnV = ampl; //New value for PWM
}

void set_motor_L(int k_curr){
	if(k_curr>=0) PTB->PDOR &= ~(1<<10); //direction
		else PTB->PDOR |=(1<<10);
	
	uint16_t	ampl;
	uint16_t	mod_curr=MOD_100Hz;
	ampl=((int)mod_curr*(100-abs(k_curr)))/100;
	TPM0->CONTROLS[5].CnV = ampl;
}
