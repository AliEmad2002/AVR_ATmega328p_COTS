/*
 * UART_program.c
 *
 * Created: 15/03/2022 01:43:12 م
 *  Author: Ali Emad
 */ 

/*
 *													N O T E S
 * 1.	UART ack: Because the USB-TTL converter used does not support synchronous mode (only asynchronous),
 *		it was important to not allow the computer to send the next data byte until it surly knows that the master
 *		MCU had successfully received the previous one, which is done by making the first wait for an acknowledgment.
 *
 */

#include "STD_TYPES.h"
#include "Bit_Math.h"
#include "CLOCK_interface.h"
#include "INTERRUPT.h"
#include "manipPIN_interface.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "UART_interface.h"
#include "UART_private.h"

//	non-blocking receive method:
static u8 bufferLen = 0;

void MUART_voidInit(u32 baudRate, enum MUARTAsynchronousSpeed speed, enum MUARTRxState Tx_State, enum MUARTRxState Rx_State, enum MUARTCharSize size, enum MUARTParity parity, enum MUARTStopBit stopBit)
{
	// setting baud rate :
	u8 factor = (speed==MUART_Asynchronous_Normal) ? 16 : 8 ;
	u16 UBRR = (u16)roundf((f32)MCLOCK_u32GetSystemClock() / (f32)(factor * baudRate)) - 1;
	UBRR0L_register = UBRR & 0x00FF ;
	UBRR0H_register = UBRR >> 8 ;
	
	// assume always Asynchronous :
	CLR_BIT(UCSR0C_register, UMSEL00) ;
	CLR_BIT(UCSR0C_register, UMSEL01) ;
	
	// write parity bits :
	WRITE_BIT(UCSR0C_register, UPM00, GET_BIT(parity, 0)) ;
	WRITE_BIT(UCSR0C_register, UPM01, GET_BIT(parity, 1)) ;
	
	// write stop-bit bit :
	WRITE_BIT(UCSR0C_register, UPM01, stopBit) ;
	
	// write char size bits :
	WRITE_BIT(UCSR0C_register, UCSZ00, GET_BIT(size, 0)) ;
	WRITE_BIT(UCSR0C_register, UCSZ01, GET_BIT(size, 1)) ;
	WRITE_BIT(UCSR0B_register, UCSZ02, GET_BIT(size, 2)) ;
	PUARTCharSize9bits = (size==MUART_CharSize_9) ? 1 : 0 ;
	
	// write Rx and Tx EN bits :
	WRITE_BIT(UCSR0B_register, RXEN0, Rx_State) ;
	WRITE_BIT(UCSR0B_register, TXEN0, Tx_State) ;
	
	// writing double baud rate bit :
	WRITE_BIT(UCSR0A_register, U2X0, speed) ;
}

void MUART_voidFastInit(u32 baudRate)
{
	// determines the more proper speed, enables Tx and Rx, uses char size of 8 bits, disables parity, uses one stop-bit.
	u32 UBRR_16 = MCLOCK_u32GetSystemClock() / (16 * baudRate) - 1;
	u32 UBRR_8 = MCLOCK_u32GetSystemClock() / (8 * baudRate) - 1;
	f32 baud_16 = (f32)MCLOCK_u32GetSystemClock() / 16.0f / (f32)(UBRR_16+1) ;
	f32 baud_8 = (f32)MCLOCK_u32GetSystemClock() / 8.0f / (f32)(UBRR_8+1) ;
	f32 deltaBaud_16 = baud_16 - (f32)baudRate ;
	deltaBaud_16 = (deltaBaud_16<0.0f) ? (-deltaBaud_16) : deltaBaud_16 ;
	f32 deltaBaud_8 = baud_8 - (f32)baudRate ;
	deltaBaud_8 = (deltaBaud_8<0.0f) ? (-deltaBaud_8) : deltaBaud_8 ;
	enum MUARTAsynchronousSpeed speed = (deltaBaud_16>deltaBaud_8) ? MUART_Asynchronous_Normal : MUART_Asynchronous_Double ;
	MUART_voidInit(baudRate, speed, MUART_Tx_EN, MUART_Rx_EN, MUART_CharSize_8, MUART_Parity_DIS, MUART_StopBit_1);
}

void MUART_VoidSenddata(u16 data)
{
	// wait for the transmitter buffer to get empty :
	while( ! GET_BIT(UCSR0A_register, UDRE0) ) ;
	
	// are we in 9 bits char size mode?
	/*if(PUARTCharSize9bits) // if so, write the 9th bit before the first 8 bits (according to datasheet)
		WRITE_BIT(UCSR0B_register, TXB80, GET_BIT(data, 8)) ;*/
		
	// write data to the transmitter buffer :
	UDR0_register = data;// & 0x00FF; //NOTICE: you can't do this before the previous step (in 9 bits mode), because writing UDR_register starts the process of sending data immediately.
}

void MUART_voidSendString (char* str) //char size must be 7-bits or 8-bits
{
	for(u8 i=0; str[i]!='\0'; i++)
	{
		//wait for the transmitter buffer to get empty :
		while( ! GET_BIT(UCSR0A_register, UDRE0) ) ;
		
		//write Data to the transmitter buffer :
		UDR0_register = str[i] ;
	}
	
	// end of the string is '\0' :
	//UDR0_register = '\0' ;
	
	// end of the string is '\r' :
	UDR0_register = '$' ;
}

u16 MUART_u16Receivedata(void)
{
	u16 data = 0;
	//wait for new unread data :
	while( ! GET_BIT(UCSR0A_register, RXC0) ) ;
	
	//are we in 9 bits char size mode?
	/*if(PUARTCharSize9bits) // if so, read the 9th bit first (according to the datasheet)
		data = GET_BIT(UCSR0B_register, RXB80) << 8 ;*/
	
	// read rest of the data from the receiver buffer :
	data = UDR0_register ;
	
	return data ;
}

void MUART_voidReceiveString (char* str, u8 maxLen) //char size must be 7-bits or 8-bits
{
	char ch ;
	u8 count = 0 ;
	while(ch!='\0' || ch!='\n')
	{
		// wait for new unread data :
		while( ! GET_BIT(UCSR0A_register, RXC0) ) ;
		
		// read data from the receiver buffer :
		ch = UDR0_register ;
		
		// foolish temp ack for the sake of non-well handled C++ computer serial lib
		MUART_VoidSenddata(';');
				
		if(ch=='\n' || ch=='\r' || ch=='\0' || count==maxLen)
		{
			str[count] = '\0' ;
			//empty Rx buffer:
			while (GET_BIT(UCSR0A_register, RXC0))
				ch = UDR0_register ;
			break;
		}
		else
			str[count++] = ch ;
	}
}

void MUART_voidEnableInterrupt (enum MUARTInterruptMode mode)
{
	INTERRUPT_ENABLE_GLOBAL_INTERRUPT ;
	SET_BIT(UCSR0B_register, mode) ;
}

void MUART_voidDisableInterrupt (enum MUARTInterruptMode mode)
{
	CLR_BIT(UCSR0B_register, mode) ;
}

void MUART_voidSetCallBack (enum MUARTInterruptMode mode, void* callBack)
{
	if(mode == MUART_Interrupt_UDR_Empty)
		PUART_CallBack_UDRE = (void(*)(void))callBack;
		
	else if (mode == MUART_Interrupt_TxComplete)
		PUART_CallBack_TXC = (void(*)(void))callBack;
		
	else if (mode == MUART_Interrupt_RxComplete)
		PUART_CallBack_RXC = (void(*)(void))callBack;
}

void PUART_VECT_UDRE (void)
{
	PUART_CallBack_UDRE();
}

void PUART_VECT_TXC (void)
{
	PUART_CallBack_TXC();
}

void PUART_VECT_RXC (void)
{
	PUART_CallBack_RXC();
	SET_BIT(UCSR0A_register, RXC0) ;
}

u8 MUART_u8SerialDataAvailable (void)
{
	if (GET_BIT(UCSR0A_register, RXC0))
		return 1 ;
	else
		return 0 ;
}

void UART_voidInitNonBlockingReceive(void)
{
	// init global variables:
	UART_globalBuffer = malloc(128 * sizeof(char));
	bufferLen = 0;
	UART_globalBufferCompleted = 0;
	
	//	assign UART callBack:
	MUART_voidSetCallBack(MUART_Interrupt_RxComplete, UART_RX_complete_callBack);
	
	//	enable interrupt:
	MUART_voidEnableInterrupt(MUART_Interrupt_RxComplete);
	
	// get data:
	MUART_VoidSenddata('$');
}

void UART_RX_complete_callBack(void)
{
	char ch = MUART_u16Receivedata();
	UART_globalBuffer[bufferLen++] = ch;
	
	if (ch == ';')
	{
		UART_globalBuffer[bufferLen-1] = '\0';
		UART_globalBufferCompleted = 1;
	}
	
	//ack:
	MUART_VoidSenddata(';');
}

void UART_doneUsingUsableBuffer_routine(void)
{
	// clear buffer:
	bufferLen = 0;
	
	// get data:
	MUART_VoidSenddata('$');
}

//void UART_voidInitNonBlockingReceive(void)
//{
	////	init UART buffers:
	//UART_globalBuffer = malloc(128 * sizeof(char));
	//UART_globalUsableBuffer = malloc(128 * sizeof(char));
	//UART_globalBuffer[0] = '\0';
	//
	////	assign UART callBack:
	//MUART_voidSetCallBack(MUART_Interrupt_RxComplete, UART_RX_complete_callBack);
	//
	////	enable interrupt:
	//MUART_voidEnableInterrupt(MUART_Interrupt_RxComplete);
//}
//
///*
 //* At receiving a new byte, it would be sent again to acknowledge (very important as CNC may fuck up
 //* for a single missed bit, parity is not an option as it leaks sometimes).
 //* Whenever an ack is sent from this MCU, the other one should send a byte that indicates: "yes, you received
 //* right", or "no, it's a miss" (the exact two bytes are previously hand-shake/agreed on).
 //*/
//void UART_RX_complete_callBack(void)
//{
	//static MUART_recivedByte_t callState =  MUART_recivedByte_msg;
	//static char chStored;
	//
	////	read new byte:
	//char ch = MUART_u16Receivedata();
	//
	//if (callState == MUART_recivedByte_msg)
	//{
		////	send it back to acknowledge:
		//MUART_VoidSenddata(ch);
		//
		//// store it temporarily:
		//chStored = ch;
		//
		////	it won't be added to the buffer until acknowledge is received on it, so wait non-blocking:
		//callState = MUART_recivedByte_ack;
	//}
	//
	//else //if (callState == MUART_recivedByte_ack)
	//{
		//if (ch == '$')	// yes message
		//{
			////	buffer the stored char:
			//UART_globalBuffer[UART_globalBufferCounter++] = chStored;
			//UART_globalBuffer[UART_globalBufferCounter] = '\0';
			//
			//// check if it is a buffer terminator:
			//if (chStored == ';')
			//{
				///*
				 //* if UART_globalUsableBuffer is already in use, it should not be edited,
				 //* neither salute should be sent, to block the sender until UART_globalUsableBuffer
				 //* is back non-used.
				 //*
				 //* once UART_globalUsableBuffer is not needed any more, user manually should check
				 //* if the UART_globalBuffer[UART_globalBufferCounter-1] is a terminator, and if so,
				 //* they should:
				 //*	- copy UART_globalBuffer to UART_globalUsableBuffer.
				 //*	- clear UART_globalUsableBufferInUse.
				 //*	- clear UART_globalBufferCounter.
				 //*	- set UART_globalBufferCompleted.
				 //*	- set UART_globalUsableBufferInUse.
				 //*	- salute the sender.
				 //*/
				//if (UART_globalUsableBufferInUse)
					//return;
				//
				//// copy UART_globalBuffer to UART_globalUsableBuffer (to avoid overwriting):
				//strcpy(UART_globalUsableBuffer, UART_globalBuffer);
				//
				//// ';' at the end is not needed:
				//UART_globalUsableBuffer[UART_globalBufferCounter-1] = '\0';
				//
				//UART_globalBufferCounter = 0;
				//UART_globalBufferCompleted = 1;
				//UART_globalUsableBufferInUse = 1;
			//}
			//
			////	salute the sender:
			//MUART_VoidSenddata('$');
		//}
		//
		//else	// no message, or the acknowledgment response its self, missed up:
		//{
			////	tell the sender to resend last msg byte:
			//MUART_VoidSenddata('#');
		//}
		//
		////	get ready for the next msg byte:
		//callState = MUART_recivedByte_msg;
	//}
//}
//
//void UART_doneUsingUsableBuffer_routine(void)
//{
	////	clear UART_globalUsableBufferInUse:
	//UART_globalUsableBufferInUse = 0;
	//
	////	if UART_globalBuffer was already filled with a cmd, but not yet copied to 
	////	UART_globalUsableBuffer, do	it:
	//if (UART_globalBuffer[strlen(UART_globalBuffer)-1] == ';')
	//{
		////	copy UART_globalBuffer to UART_globalUsableBuffer:
		//strcpy(UART_globalUsableBuffer, UART_globalBuffer);
		//
		//// ';' at the end is not needed:
		//UART_globalUsableBuffer[UART_globalBufferCounter-1] = '\0';
		//
		////	clear UART_globalBufferCounter:
		//UART_globalBufferCounter = 0;
		//
		////	set UART_globalBufferCompleted:
		//UART_globalBufferCompleted = 1;
		//
		////	salute the sender.
		//MUART_VoidSenddata('$');
	//}
	//
//}