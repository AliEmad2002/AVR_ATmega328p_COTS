/*
 * UART_private.h
 *
 * Created: 15/03/2022 01:40:32 م
 *  Author: Ali Emad
 */ 


#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_


// registers and thier bits :
#define UDR0_register   ( *(volatile u8 *) 0xC6 )

#define UBRR0H_register ( *(volatile u8 *) 0xC5 )
#define UBRR0L_register ( *(volatile u8 *) 0xC4 )

#define UCSR0C_register ( *(volatile u8 *) 0xC2 )
#define UCPOL0 0
#define UCSZ00 1
#define UCSZ01 2
#define USBS0 3
#define UPM00 4
#define UPM01 5
#define UMSEL00 6
#define UMSEL01 7

#define UCSR0B_register ( *(volatile u8 *) 0xC1 )
#define TXB80 0
#define RXB80 1
#define UCSZ02 2
#define TXEN0 3
#define RXEN0 4
#define UDRIE0 5
#define TXCIE0 6
#define RXCIE0 7

#define UCSR0A_register ( *(volatile u8 *) 0xC0 )
#define MPCM0 0
#define U2X0 1
#define UPE0 2
#define DOR0 3
#define FE0 4
#define UDRE0 5
#define TXC0 6
#define RXC0 7

#define PUART_VECT_RXC __vector_18
#define PUART_VECT_UDRE __vector_19
#define PUART_VECT_TXC __vector_20


void PUART_VECT_UDRE(void) __attribute__((signal, used));
void PUART_VECT_TXC(void) __attribute__((signal, used));
void PUART_VECT_RXC(void) __attribute__((signal, used));

void (*PUART_CallBack_UDRE)(void);
void (*PUART_CallBack_TXC)(void);
void (*PUART_CallBack_RXC)(void);


u8 PUARTCharSize9bits;
char* PUARTRecivedMsg ;



#endif /* UART_PRIVATE_H_ */