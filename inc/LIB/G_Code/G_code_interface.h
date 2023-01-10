/*
 * G_code_interface.h
 *
 * Created: 7/13/2022 9:54:24 PM
 *  Author: Ali Emad
 */ 


#ifndef G_CODE_INTERFACE_H_
#define G_CODE_INTERFACE_H_

enum G_CODE_G_COEDES {
	G_CODE_rapidMovement =			0,
	G_CODE_feedMovement =			1,
	G_CODE_autoLeveling =			29,
	G_CODE_singleProbe =			30,
	G_CODE_abslutePositioning =		90,
	G_CODE_relativePositioning =	91,
	G_CODE_softwareSetPosition =	92
	};
	
enum G_CODE_M_COEDES {
	G_CODE_setMaxFeedrate =			203,
	G_CODE_setAcceleration =		204,
	G_CODE_enableAutoLeveling =		420
};

#define G_CODE_u16ARRTERMINATOR		9999
#define G_CODE_EXCUTABLE_G_CODES ((u16[]){G_CODE_rapidMovement, G_CODE_feedMovement, G_CODE_autoLeveling, G_CODE_softwareSetPosition, G_CODE_u16ARRTERMINATOR})
#define G_CODE_EXCUTABLE_M_CODES ((u16[]){G_CODE_enableAutoLeveling, G_CODE_u16ARRTERMINATOR})

s16 str_find(char* str, char ch, s16 start);

f32 str_to_float(char* str, s16 start, s16 end);

u8 find_number_ending(char* line, u8 start);

void G_Code_voidCopyPoint(s32* point, char paramCh[6], f32 paramNum[6], u8 paramCount);

void G_Code_voidCopyPointAL(s32* point, char paramCh[6], f32 paramNum[6]);

void G_CODE_voidCopyAcceleration(u32** mainParametersPtrArr, char paramCh[6], f32 paramNum[6], u8 paramCount);

void G_Code_voidParseLine(char* codeClass, u16* code, s32* point, u32* feedRate, u32** mainParametersPtrArr, char* line);

u8 G_CODE_u8IsExecutableCode(char codeClass, u16 code);


#endif /* G-CODE_INTERFACE_H_ */