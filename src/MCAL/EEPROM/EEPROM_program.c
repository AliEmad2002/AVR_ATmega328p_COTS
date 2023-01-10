/*
 * EEPROM_program.c
 *
 * Created: 7/29/2022 4:57:18 PM
 *  Author: ali20
 */ 


#include "Bit_Math.h"
#include "STD_TYPES.h"


#include "EEPROM_interface.h"
#include "EEPROM_private.h"


void EEPROM_voidWriteByte(u16 address, u8 data)
{
	/* wait for previous write to complete	*/
	while(GET_BIT(EECR_register, EEPE));
	
	/* write address in the address register	*/
	EEAR_register = address;
	
	/*	write data to EEDR	*/
	EEDR_register = data;
	
	/*	enable master write	*/
	SET_BIT(EECR_register, EEMPE);
	
	/*	copy data of EEDR to the pointed register	*/
	SET_BIT(EECR_register, EEPE);
}


void EEPROM_voidWrite2Bytes(u16 address, u16 data)
{
	EEPROM_voidWriteByte(address,	(u8)data);
	EEPROM_voidWriteByte(address+1, (u8)(data>>8));
}

void EEPROM_voidWrite4Bytes(u16 address, u32 data)
{
	EEPROM_voidWriteByte(address,	(u8)data);
	EEPROM_voidWriteByte(address+1, (u8)(data>>8));
	EEPROM_voidWriteByte(address+2, (u8)(data>>16));
	EEPROM_voidWriteByte(address+3, (u8)(data>>24));
}

void EEPROM_voidWrite8Bytes(u16 address, u64 data)
{
	for (u8 i=0; i<8; i++)
		EEPROM_voidWriteByte(address+i, (u8)(data>>(i*8)));
}

u8 EEPROM_u8ReadByte(u16 address)
{
	/* wait for previous write to complete	*/
	while(GET_BIT(EECR_register, EEPE));
	
	/* write address in the address register	*/
	EEAR_register = address;
	
	/*	copy data of the pointed register to EEDR	*/
	SET_BIT(EECR_register, EERE);
	
	return EEDR_register;
}

u16 EEPROM_u16Read2Bytes(u16 address)
{
	return
		(u16)EEPROM_u8ReadByte(address)				|
		(((u16)EEPROM_u8ReadByte(address+1))<<8);
}

u32 EEPROM_u32Read4Bytes(u16 address)
{
	return
		(u32)EEPROM_u8ReadByte(address)				|
		(((u32)EEPROM_u8ReadByte(address+1))<<8)	|
		(((u32)EEPROM_u8ReadByte(address+2))<<16)	|
		(((u32)EEPROM_u8ReadByte(address+3))<<24);
}

u64 EEPROM_u32Read8Bytes(u16 address)
{
	return
		(u64)EEPROM_u8ReadByte(address)				|
		(((u64)EEPROM_u8ReadByte(address+1))<<8)	|
		(((u64)EEPROM_u8ReadByte(address+2))<<16)	|
		(((u64)EEPROM_u8ReadByte(address+3))<<24)	|
		(((u64)EEPROM_u8ReadByte(address+4))<<32)	|
		(((u64)EEPROM_u8ReadByte(address+5))<<40)	|
		(((u64)EEPROM_u8ReadByte(address+6))<<48)	|
		(((u64)EEPROM_u8ReadByte(address+7))<<56);
}