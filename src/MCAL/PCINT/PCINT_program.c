/*
 * PCINT_program.c
 *
 * Created: 8/13/2022 11:37:29 PM
 *  Author: Ali Emad Ali
 */ 

/*	LIB	*/
#include "STD_Types.h"
#include "Bit_Math.h"
/*	MCAL	*/
#include "INTERRUPT.h"
#include "PCINT_private.h"
#include "PCINT_interface.h"

#ifndef NULL
#define NULL	(void*)0x00
#endif

static void (*PCINT_CallBack[3])(void) = {NULL, NULL};


void MPCINT_voidEnableChannel(MPCINT_t channel)
{
	/*	enable PCINTx	*/
	SET_BIT(PCICR_REG, channel);
	
	/*	enable global interrupt	*/
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void MPCINT_voidDisableChannel(MPCINT_t channel)
{
	/*	disable PCINTx	*/
	CLR_BIT(PCICR_REG, channel);
}

void MPCINT_voidMaskPin(MPCINT_PIN_t pin)
{
	/*	calculate channel	*/
	MPCINT_t channel = pin / 8;
	
	/*	set bit PCINTx in PCMSKn, where x is "pin", and n is "channel"	*/
	SET_BIT(PCMSK_REG_ARR[channel], pin % 8);
}

void MPCINT_voidUnmaskPin(MPCINT_PIN_t pin)
{
	/*	calculate channel	*/
	MPCINT_t channel = pin / 8;
	
	/*	clear bit PCINTx in PCMSKn, where x is "pin", and n is "channel"	*/
	CLR_BIT(PCMSK_REG_ARR[channel], pin % 8);
}

void MPCINT_voidSetChannelCallBack(MPCINT_t channel, void(*callBack)(void))
{
	PCINT_CallBack[channel] = callBack;
}

void PCICR_VECT_ISR_PCINT0(void)
{
	if (PCINT_CallBack[0] != NULL)
		PCINT_CallBack[0]();
	
	/*
	 * even though interrupt flag is cleared by HW at executing ISR, it could be set again during that execution,
	 * if this was due to physical unwanted phenomena, like button bouncing, one way to avoid it is by clearing
	 * the interrupt flag on leaving the ISR by SW, other wise it would be left set because of that phenomena,
	 * and ISR is to be re-executed on leave.
	 */ 
	SET_BIT(PCIFR_REG, 0);
	
	/*
	 * Re-Enable global interrupt as it gets disabled by HW whenever an EXTI-SR is executed
	 * (ATmega328p datasheet page 20: 7.3.1)
	 */
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void PCICR_VECT_ISR_PCINT1(void)
{
	if (PCINT_CallBack[1] != NULL)
		PCINT_CallBack[1]();
	
	/*
	 * even though interrupt flag is cleared by HW at executing ISR, it could be set again during that execution,
	 * if this was due to physical unwanted phenomena, like button bouncing, one way to avoid it is by clearing
	 * the interrupt flag on leaving the ISR by SW, other wise it would be left set because of that phenomena,
	 * and ISR is to be re-executed on leave.
	 */ 
	SET_BIT(PCIFR_REG, 1);
		
	/*
	 * Re-Enable global interrupt as it gets disabled by HW whenever an EXTI-SR is executed
	 * (ATmega328p datasheet page 20: 7.3.1)
	 */
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void PCICR_VECT_ISR_PCINT2(void)
{
	if (PCINT_CallBack[2] != NULL)
		PCINT_CallBack[2]();
	
	/*
	 * even though interrupt flag is cleared by HW at executing ISR, it could be set again during that execution,
	 * if this was due to physical unwanted phenomena, like button bouncing, one way to avoid it is by clearing
	 * the interrupt flag on leaving the ISR by SW, other wise it would be left set because of that phenomena,
	 * and ISR is to be re-executed on leave.
	 */ 
	SET_BIT(PCIFR_REG, 2);
	
	/*
	 * Re-Enable global interrupt as it gets disabled by HW whenever an EXTI-SR is executed
	 * (ATmega328p datasheet page 20: 7.3.1)
	 */
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}