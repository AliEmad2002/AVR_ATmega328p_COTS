/*
 * ADC_program.c
 *
 * Created: 18/09/2021 06:29:37 م
 *  Author: Ali Emad
 */
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "INTERRUPT.h"

#include "ADC_interface.h"
#include "ADC_private.h"
//#include <avr/io.h>


void MADC_voidInit(enum ADC_Mode_t Mode, enum ADC_Prescalar_t Prescaler)
{
	//in ADMUX register:
	//1-Reference Selection: AVcc
	ADMUX_register = (1<<REFS0_bit);
	
	//in ADCSRA register:
	//1-ADC Enable
	//2-ADC Prescaler Select Bits : (depends on the "Prescaler")
	ADCSRA_register = 0b10000000 | (Mode<<ADATE_bit) | Prescaler ;
	//3-ADC Start Conversion : (performing a NULL first conversion to get rid of the 25 cycles)
	ADCSRA_register |= 1<<ADSC_bit ;
	
	//Notice that "Prescaler" controls speed of the ADC which effects the ADC resolution (ie: number of bits the value is going to be written in) 
	
	
}

void MADC_voidSetAutoTriggerSource(enum ADC_AutoTrigger_t TriggerSource)
{
	//in SFIOR register :
	SFIOR_register |= TriggerSource<<ADTS0_bit;
}

u16 MADC_u16ReadChannelSynch(enum ADC_ChannelID_t Channel)
{
	//Write channel value in MUX4:MUX0 bits in ADMUX_register :
	ADMUX_register = (ADMUX_register & 0b11100000)|Channel; // clears the bottom 5 bits before writing new Channel
	
	//Start conversion by setting ADSC_bit in ADCSRA_register to 1 logic:
	ADCSRA_register |= 1<<ADSC_bit;
	
	//Wait for the conversion to be done:
	while(ADCSRA_register & (1<<ADSC_bit));
	
	//After the conversion is done, read ADCL and ADCH and return them:
	u16 read = (u16)ADCL_register | (u16)ADCH_register<<8;
	return read;
}

void MADC_voidReadChannelAsynch(enum ADC_ChannelID_t Channel, void (*CallBack)(u16))
{
	//Write channel value in MUX4:MUX0 bits in ADMUX_register :
	ADMUX_register &= 0b11100000; //erase any previous value in these 5 bits while keeping the ADLAR, REFS0 and REFS1 bits as they are
	ADMUX_register |= Channel;	  //Write the new channel value.
	
	//Assign "CallBack" to "PADC_CallBack_Complete" function:
	PADC_CallBack_Complete = CallBack;
	
	//Clear ADC Interrupt Flag by setting it to logic 1, and enable ADC interrupt:
	ADCSRA_register |= 0b11<<ADIE_bit ;
	
	//Enable global interrupt :
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT;
	
	//Start conversion by setting ADSC_bit in ADCSRA_register to logic 1:
	ADCSRA_register |= 1<<ADSC_bit;
}

void MADC_voidStartContinousMode(void)//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// right?
{
	ADCSRA_register |= 1<<ADATE_bit ;
} 

void MADC_voidStopContinousMode(void)
{
	ADCSRA_register &= ~(1<<ADATE_bit) ;
}

void PADC_VECT_ISR_COMPLETE (void)//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// works only once! why???
{
	/*//After the conversion is done, read ADCL and ADCH and give it to PADC_CallBack_Complete function as an argument:
	u16 read = (u16)ADCL_register | (u16)ADCH_register<<8;
	PADC_CallBack_Complete(read);*/
}