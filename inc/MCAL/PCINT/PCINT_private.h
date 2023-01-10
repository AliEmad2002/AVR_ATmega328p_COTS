/*
 * PCINT_private.h
 *
 * Created: 8/13/2022 11:37:15 PM
 *  Author: Ali Emad Ali
 */ 


#ifndef PCINT_PRIVATE_H_
#define PCINT_PRIVATE_H_

#define PCICR_REG		(*(volatile u8*)0x68)
#define PCMSK_REG_ARR	((volatile u8*)0x6B)
#define PCMSK0_REG		(*(volatile u8*)0x6B)
#define PCMSK1_REG		(*(volatile u8*)0x6C)
#define PCMSK2_REG		(*(volatile u8*)0x6D)
#define PCIFR_REG		(*(volatile u8*)0x3B)


#define PCICR_VECT_ISR_PCINT0 __vector_3
#define PCICR_VECT_ISR_PCINT1 __vector_4
#define PCICR_VECT_ISR_PCINT2 __vector_5



#endif /* PCINT_PRIVATE_H_ */