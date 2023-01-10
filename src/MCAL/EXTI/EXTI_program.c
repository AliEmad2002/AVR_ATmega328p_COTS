/*
 * EXTI_program.c
 *
 * Created: 8/13/2022 4:37:03 PM
 *  Author: Ali Emad Ali
 */ 

/*	LIB	*/
#include "STD_Types.h"
#include "Bit_Math.h"
/*	MCAL	*/
#include "INTERRUPT.h"
#include "EXTI_private.h"
#include "EXTI_interface.h"

#ifndef NULL
#define NULL	(void*)0x00
#endif

static void (*EXTI_CallBack[2])(void) = {NULL, NULL};


void MEXTI_voidEnable(u8 intNum, EXTI_SENSE_t senseMode)
{
	/* Range check	*/
	if (intNum < 0 || intNum > 1)
	{
		/* wrong intNum	*/
		while(1);
	}
	
	/* set sense mode	*/
	WRITE_BIT(EICRA_REG, 2*intNum, GET_BIT(senseMode, 0));
	WRITE_BIT(EICRA_REG, 2*intNum + 1, GET_BIT(senseMode, 1));
	
	/*	set INTx bit to 1	*/
	SET_BIT(EIMSK_REG, intNum);
	
	/*	Enable global interrupt	*/
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void MEXTI_voidDisable(u8 intNum)
{
	/* Range check	*/
	if (intNum < 0 || intNum > 1)
	{
		/* wrong intNum	*/
		while(1);
	}
	
	/*	clear INTx bit to 0	*/
	CLR_BIT(EIMSK_REG, intNum);
}

void MEXTI_voidSetCallBack(u8 intNum, void(*callBack)(void))
{
	/* Range check	*/
	if (intNum < 0 || intNum > 1)
	{
		/* wrong intNum	*/
		while(1);
	}
	
	EXTI_CallBack[intNum] = callBack;
}

void EXTI_VECT_ISR_INT0(void)
{
	if (EXTI_CallBack[0] != NULL)
		EXTI_CallBack[0]();
	
	/*
	 * even though interrupt flag is cleared by HW at executing ISR, it could be set again during that execution,
	 * if this was due to physical unwanted phenomena, like button bouncing, one way to avoid it is by clearing
	 * the interrupt flag on leaving the ISR by SW, other wise it would be left set because of that phenomena,
	 * and ISR is to be re-executed on leave.
	 */ 
	SET_BIT(EIFR_REG, 0);
	
	/*
	 * Re-Enable global interrupt as it gets disabled by HW whenever an EXTI-SR is executed
	 * (ATmega328p datasheet page 20: 7.3.1)
	 */
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

void EXTI_VECT_ISR_INT1(void)
{
	if (EXTI_CallBack[1] != NULL)
		EXTI_CallBack[1]();
	
	/*
	 * even though interrupt flag is cleared by HW at executing ISR, it could be set again during that execution,
	 * if this was due to physical unwanted phenomena, like button bouncing, one way to avoid it is by clearing
	 * the interrupt flag on leaving the ISR by SW, other wise it would be left set because of that phenomena,
	 * and ISR is to be re-executed on leave.
	 */ 
	SET_BIT(EIFR_REG, 1);
	
	/*
	 * Re-Enable global interrupt as it gets disabled by HW whenever an EXTI-SR is executed
	 * (ATmega328p datasheet page 20: 7.3.1)
	 */
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
}

