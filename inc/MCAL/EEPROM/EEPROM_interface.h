/*
 * EEPROM_interface.h
 *
 * Created: 7/29/2022 4:57:31 PM
 *  Author: ali20
 */ 


#ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

void EEPROM_voidWriteByte(u16 address, u8 data);

void EEPROM_voidWrite2Bytes(u16 address, u16 data);

void EEPROM_voidWrite4Bytes(u16 address, u32 data);

void EEPROM_voidWrite8Bytes(u16 address, u64 data);

u8 EEPROM_u8ReadByte(u16 address);

u16 EEPROM_u16Read2Bytes(u16 address);

u32 EEPROM_u32Read4Bytes(u16 address);

u64 EEPROM_u32Read8Bytes(u16 address);

#endif /* EEPROM_INTERFACE_H_ */