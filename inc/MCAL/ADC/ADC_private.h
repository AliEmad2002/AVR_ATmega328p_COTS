/*
 * ADC_private.h
 *
 * Created: 18/09/2021 08:33:33 م
 *  Author: Ali Emad
 */ 


#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

#define PADC_VECT_ISR_COMPLETE __vector_14 //(14 on atmega16), change it to ?? on atmega32

void PADC_VECT_ISR_COMPLETE(void) __attribute__((signal, used));

void (*PADC_CallBack_Complete)(u16);

#define ADMUX_register    ( *(volatile u8 *) 0x27 )
#define ADCSRA_register   ( *(volatile u8 *) 0x26 )
#define SFIOR_register    ( *(volatile u8 *) 0x50 )
#define ADCL_register     ( *(volatile u8 *) 0x24 )
#define ADCH_register     ( *(volatile u8 *) 0x25 )


#define ADATE_bit 5
#define ADTS0_bit 5
#define ADSC_bit  6
#define ADIE_bit  3
#define ADIF_bit  4


#endif /* ADC_PRIVATE_H_ */