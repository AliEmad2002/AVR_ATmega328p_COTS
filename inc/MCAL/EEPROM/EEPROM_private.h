/*
 * EEPROM_private.h
 *
 * Created: 7/29/2022 4:57:41 PM
 *  Author: ali20
 */ 


#ifndef EEPROM_PRIVATE_H_
#define EEPROM_PRIVATE_H_


#define EEARH_register		(*(volatile u8*)	0x42)
#define EEARL_register		(*(volatile u8*)	0x41)
#define EEAR_register		(*(volatile u16*)	0x41)
#define EEDR_register		(*(volatile u8*)	0x40)
#define EECR_register		(*(volatile u8*)	0x3F)


#define EERE				0
#define EEPE				1
#define EEMPE				2
#endif /* EEPROM_PRIVATE_H_ */