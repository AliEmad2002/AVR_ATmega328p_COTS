/*
 * PCINT_interface.h
 *
 * Created: 8/13/2022 11:37:04 PM
 *  Author: Ali Emad Ali
 */ 


#ifndef PCINT_INTERFACE_H_
#define PCINT_INTERFACE_H_


typedef enum{
	MPCINT_0,
	MPCINT_1,
	MPCINT_2
}MPCINT_t;

typedef enum{
	MPCINT_PIN_0	=	0,
	MPCINT_PIN_1	=	1,
	MPCINT_PIN_2	=	2,
	MPCINT_PIN_3	=	3,
	MPCINT_PIN_4	=	4,
	MPCINT_PIN_5	=	5,
	MPCINT_PIN_6	=	6,
	MPCINT_PIN_7	=	7,
	MPCINT_PIN_8	=	8,
	MPCINT_PIN_9	=	9,
	MPCINT_PIN_10	=	10,
	MPCINT_PIN_11	=	11,
	MPCINT_PIN_12	=	12,
	MPCINT_PIN_13	=	13,
	MPCINT_PIN_14	=	14,
	//MPCINT_PIN_15	=	15,		not available
	MPCINT_PIN_16	=	16,
	MPCINT_PIN_17	=	17,
	MPCINT_PIN_18	=	18,
	MPCINT_PIN_19	=	19,
	MPCINT_PIN_20	=	20,
	MPCINT_PIN_21	=	21,
	MPCINT_PIN_22	=	22,
	MPCINT_PIN_23	=	23
}MPCINT_PIN_t;

void MPCINT_voidEnableChannel(MPCINT_t channel);

void MPCINT_voidDisableChannel(MPCINT_t channel);

void MPCINT_voidMaskPin(MPCINT_PIN_t pin);

void MPCINT_voidUnmaskPin(MPCINT_PIN_t pin);

void MPCINT_voidSetChannelCallBack(MPCINT_t channel, void(*callBack)(void));

#endif /* PCINT_INTERFACE_H_ */