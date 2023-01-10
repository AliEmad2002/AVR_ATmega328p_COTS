/*
 * UART_interface.h
 *
 * Created: 15/03/2022 02:33:39 م
 *  Author: Ali Emad
 */ 


#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

enum MUARTTxState {MUART_Tx_DIS, MUART_Tx_EN};

enum MUARTRxState {MUART_Rx_DIS, MUART_Rx_EN};
	
enum MUARTCharSize {MUART_CharSize_5, MUART_CharSize_6, MUART_CharSize_7, MUART_CharSize_8, MUART_CharSize_9=7};
	
enum MUARTParity {MUART_Parity_DIS, MUART_Parity_Even=2, MUART_Parity_Odd};
	
enum MUARTStopBit {MUART_StopBit_1, MUART_StopBit_2};

enum MUARTInterruptMode {MUART_Interrupt_UDR_Empty=5, MUART_Interrupt_TxComplete, MUART_Interrupt_RxComplete};

enum MUARTAsynchronousSpeed {MUART_Asynchronous_Normal, MUART_Asynchronous_Double};
	
typedef enum {
	MUART_recivedByte_msg,
	MUART_recivedByte_ack
} MUART_recivedByte_t;

void MUART_voidInit(u32 baudRate, enum MUARTAsynchronousSpeed speed, enum MUARTRxState Tx_State, enum MUARTRxState Rx_State, enum MUARTCharSize size, enum MUARTParity parity, enum MUARTStopBit stopBit);

void MUART_voidFastInit(u32 baudRate); //Enables Rx and Tx, no parity, char size is 8-Bits, 1 stop bit

void MUART_VoidSenddata(u16 data); //this function will only send first n of bits in variable Data, where n is the char size selected in the Init function.

void MUART_voidSendString (char* str); //char size must be 7-bits or 8-bits

u16 MUART_u16Receivedata(void);

void MUART_voidReceiveString (char* str, u8 maxLen); //char size must be 7-bits or 8-bits

void MUART_voidEnableInterrupt (enum MUARTInterruptMode Mode);

void MUART_voidDisableInterrupt (enum MUARTInterruptMode Mode);

void MUART_voidSetCallBack (enum MUARTInterruptMode Mode, void* CallBack);

u8 MUART_u8SerialDataAvailable (void);


//	non-blocking receive method:
char* UART_globalBuffer;
u8 UART_globalBufferCompleted;

void UART_voidInitNonBlockingReceive(void);
void UART_RX_complete_callBack(void);
void UART_doneUsingUsableBuffer_routine(void);


#endif /* UART_INTERFACE_H_ */