/*
 * EXTI_interface.h
 *
 * Created: 8/13/2022 4:37:20 PM
 *  Author: Ali Emad Ali
 */ 


#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

typedef enum{
	EXTI_SENSE_low		=	0b00,
	EXTI_SENSE_toggle	=	0b01,
	EXTI_SENSE_falling	=	0b10,
	EXTI_SENSE_rising	=	0b11,
}EXTI_SENSE_t;


void MEXTI_voidEnable(u8 intNum, EXTI_SENSE_t senseMode);

void MEXTI_voidDisable(u8 intNum);

void MEXTI_voidSetCallBack(u8 intNum, void(*callBack)(void));


#endif /* EXTI_INTERFACE_H_ */