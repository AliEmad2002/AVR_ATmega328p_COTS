/*
 * AccelStepper_interface.h
 *
 * Created: 7/11/2022 7:47:43 PM
 *  Author: Ali Emad
 */ 


#ifndef ACCELSTEPPER_INTERFACE_H_
#define ACCELSTEPPER_INTERFACE_H_

#define X	0
#define Y	1
#define Z	2

typedef struct
{
	u8 stepPin;
	volatile u8* stepPortPtr;
	
	u8 dirPin;
	volatile u8* dirPortPtr;
	
	u64 lastTimeStamp;
	
	s32 currentPos;
}AccelStepper_t;

typedef enum {HAccelStepper_dirForward, HAccelStepper_dirBackward}HAccelStepper_dir;

void HAccelStepper_voidInit(AccelStepper_t* stepperPtr, u8 _stepPin, volatile u8* _stepPortPtr, u8 _dirPin, volatile u8* _dirPortPtr);

void HAccelStepper_voidStep(AccelStepper_t* stepperPtr, HAccelStepper_dir dir, u64 currentTime);

#endif /* ACCELSTEPPER_INTERFACE_H_ */