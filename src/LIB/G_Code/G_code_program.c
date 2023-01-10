/*
 * G_code_program.c
 *
 * Created: 7/13/2022 9:54:36 PM
 *  Author: Ali Emad
 */ 

/*	LIB	*/
#include "STD_TYPES.h"
/*	MCAL	*/
#include "UART_interface.h"
/*	APP	*/
#include "CNC_config.h"
#include "CNC_interface.h"
/*	self	*/
#include "G_code_interface.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s16 str_find(char* str, char ch, s16 start)
{
	for (s16 i=start; str[i]!='\0'; i++)
		if (str[i] == ch)
			return i;
			
	return -1;
}

f32 str_to_float(char* str, s16 start, s16 end)
{
	f32 n = 0.0f;

	u8 negative = 0;
	if (str[start] == '-')
	{
		negative = 1;
		start++;
	}

	u8 point = 0;
	s16 pointIndex;
	for (pointIndex = start; pointIndex <= end; pointIndex++)
	{
		if (str[pointIndex] == '.')
		{
			point = 1;
			break;
		}
		else if (str[pointIndex] == 'e')
			break;
	}
	
	u8 e = 0;
	s16 eIndex;
	for (eIndex = end - 1; eIndex > start; eIndex--)
		if (str[eIndex] == 'e')
		{
			e = 1;
			break;
		}
	if (!e)
		eIndex = end + 1;
	
	f32 decadeCounter = 1.0;
	for (s16 i = pointIndex - 1; i >= start; i--)
	{
		n += decadeCounter * (f32)(str[i] - '0');
		decadeCounter *= 10.0;
	}

	if (point)
	{
		f32 fDecadeCounter = 0.1;
		for (s16 i = pointIndex + 1; i <= eIndex - 1; i++)
		{
			n += fDecadeCounter * (f32)(str[i] - '0');
			fDecadeCounter /= 10.0;
		}
	}
	
	if (e)
	{
		f32 power = str_to_float(str, eIndex + 1, end);
		n *= (f32)pow(10.0, power);
	}
	
	return n * (negative ? -1.0 : 1.0);
}

/*	returns the index of the next space - if there's one -, otherwise returns the size of the line string	*/
u8 find_number_ending(char* line, u8 start)
{
	while(line[start] != ' '		&&		line[start] != '\0')
		start++;
		
	return start - 1;
}

void G_Code_voidCopyPoint(s32* point, char paramCh[6], f32 paramNum[6], u8 paramCount)
{
	while(paramCount--)
	{
		u8 pointIndex = paramCh[paramCount] - 'X';
		point[pointIndex] = paramNum[paramCount] * (f32)STEPS_PER_MM;
	}
}

void G_Code_voidCopyPointAL(s32* point, char paramCh[6], f32 paramNum[6])
{
	point[4] = paramNum[4];
	
	u8 i = 4;
	while(i--)
	{
		point[i] = paramNum[i] * (f32)STEPS_PER_MM;
	}
}

void G_CODE_voidCopyAcceleration(u32** mainParametersPtrArr, char paramCh[6], f32 paramNum[6], u8 paramCount)
{
	while(paramCount--)
	{
		if (paramCh[paramCount] == 'P')	// feed
		{
			*mainParametersPtrArr[CNC_accelerationFeed] = paramNum[paramCount] * STEPS_PER_MM;
		}
		else // if (paramCh[paramCount] == 'T')	// rapid
		{
			*mainParametersPtrArr[CNC_accelerationRapid] = paramNum[paramCount] * STEPS_PER_MM;
		}
	}
}

void G_Code_voidParseLine(char* codeClass, u16* code, s32* point, u32* feedRate, u32** mainParametersPtrArr, char* line)
{
	/*	declaring variables	*/
	u8 endIndex;
	// assuming that maximum params per cmd is 6.
	char paramCh[6];
	f32 paramNum[6];
	
	/*	get code class	*/
	*codeClass = line[0];
	
	/*	get code number	*/
	endIndex = find_number_ending(line, 2);
	*code = str_to_float(line, 1, endIndex);
	
	/*	get parameters	*/
	u8 paramCount = 0;
	while(line[++endIndex] != '\0')
	{
		paramCh[paramCount] = line[++endIndex];
		u8 tempEndIndex = endIndex + 1;
		endIndex = find_number_ending(line, tempEndIndex);
		paramNum[paramCount++] = str_to_float(line, tempEndIndex, endIndex);
	}
	
	if (*codeClass == 'G')
	{
		switch (*code)
		{
			case G_CODE_rapidMovement:
				G_Code_voidCopyPoint(point, paramCh, paramNum, paramCount);
				break;
			
			case G_CODE_feedMovement:
				G_Code_voidCopyPoint(point, paramCh, paramNum, paramCount);
				*feedRate = paramNum[paramCount-1] * STEPS_PER_MM;
				break;
				
			case G_CODE_abslutePositioning:
				*mainParametersPtrArr[CNC_relativePositioning] = 0;
				break;
				
			case G_CODE_relativePositioning:
				*mainParametersPtrArr[CNC_relativePositioning] = 1;
				break;
				
			case G_CODE_autoLeveling:
				G_Code_voidCopyPointAL(point, paramCh, paramNum);
				break;
				
			case G_CODE_softwareSetPosition:
				G_Code_voidCopyPoint(point, paramCh, paramNum, paramCount);
				break;
		}
	}
	
	else if (*codeClass == 'M')
	{	
		switch (*code)
		{				
			case G_CODE_setMaxFeedrate:
				*mainParametersPtrArr[CNC_maxFeedrate] = paramNum[0] * STEPS_PER_MM;
				break;
				
			case G_CODE_setAcceleration:
				G_CODE_voidCopyAcceleration(mainParametersPtrArr, paramCh, paramNum, paramCount);
				break;
				
			case G_CODE_enableAutoLeveling:
				break;
		}
	}
	
	else
	{
		while(1);
	}
}

u8 G_CODE_u8IsExecutableCode(char codeClass, u16 code)
{
	if (codeClass == 'G')
	{
		for (u8 i=0; G_CODE_EXCUTABLE_G_CODES[i] != G_CODE_u16ARRTERMINATOR; i++)
		{
			if (code == G_CODE_EXCUTABLE_G_CODES[i])
				return 1;
		}
	}
	
	else if (codeClass == 'M')
	{
		for (u8 i=0; G_CODE_EXCUTABLE_M_CODES[i] != G_CODE_u16ARRTERMINATOR; i++)
		{
			if (code == G_CODE_EXCUTABLE_M_CODES[i])
				return 1;
		}
	}
	
	return 0;
}