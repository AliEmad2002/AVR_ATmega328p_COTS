/*
 * EXTI_private.h
 *
 * Created: 8/13/2022 8:01:57 PM
 *  Author: ali20
 */ 


#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_


#define EICRA_REG		(*(volatile u8*)0x69)
#define EIMSK_REG		(*(volatile u8*)0x3D)
#define EIFR_REG		(*(volatile u8*)0x3C)


#define EXTI_VECT_ISR_INT0 __vector_1
#define EXTI_VECT_ISR_INT1 __vector_2


#endif /* EXTI_PRIVATE_H_ */