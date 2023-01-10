/*
 * AccelStepper_program.c
 *
 * Created: 7/11/2022 8:11:42 PM
 *  Author: Ali Emad
 */ 

/*
 *													N O T E S
 * 1.	Not using DIO driver: for speed of access as the MCU could get too over headed in some cases (i.e.: using
 *		1/32 microstepping settings).
 */


/*	LIB	*/
#include "STD_TYPES.h"
#include "Bit_Math.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/*	MCAL	*/
#include "TIMER_interface.h"
#include "CLOCK_interface.h"
#include "UART_interface.h"
/*	HAL	*/
#include "LCD_interface.h"
#include "AccelStepper_config.h"
#include "AccelStepper_interface.h"


void HAccelStepper_voidInit(
	AccelStepper_t* stepperPtr, u8 _stepPin, volatile u8* _stepPortPtr, u8 _dirPin, volatile u8* _dirPortPtr
	)
{
	// store data:
	stepperPtr->stepPin = _stepPin;
	stepperPtr->stepPortPtr = _stepPortPtr;
	// init step pin as output:
	SET_BIT(*(_stepPortPtr-1), _stepPin);
	
	// store data:
	stepperPtr->dirPin = _dirPin;
	stepperPtr->dirPortPtr = _dirPortPtr;
	// init dir pin as output:
	SET_BIT(*(_dirPortPtr-1), _dirPin);
	
	// init parameters:
	stepperPtr->lastTimeStamp = 0;
	stepperPtr->currentPos = 0;
}

void HAccelStepper_voidStep(AccelStepper_t* stepperPtr, HAccelStepper_dir dir, u64 currentTime)
{
	// output prober dir signal:
	switch (dir)
	{
		case HAccelStepper_dirForward:
			SET_BIT(*(stepperPtr->dirPortPtr), stepperPtr->dirPin);
			stepperPtr->currentPos++;
			break;
		default:
			CLR_BIT(*(stepperPtr->dirPortPtr), stepperPtr->dirPin);
			stepperPtr->currentPos--;
	}
	
	// order motors' controller to step:
	SET_BIT(*(stepperPtr->stepPortPtr), stepperPtr->stepPin);
	MCLOCK_voidDelay2USAt16MHz();
	CLR_BIT(*(stepperPtr->stepPortPtr), stepperPtr->stepPin);
	
	// refresh timeStamp:
	stepperPtr->lastTimeStamp = currentTime;
}