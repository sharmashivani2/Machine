/*
 * usart.c
 *
 * Created: 1/1/2020 11:33:31 AM
 *  Author: trans
 */ 
/*
 * usart.c
 *
 * Created: 10/3/2019 5:14:07 PM
 *  Author: trans
 */ 
#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
//#define USART_BAUDRATE 2400
#define BAUD_PRESCALE 103

void UART_init()
{
	//UBRRL=0X00;
	//UBRRH= 0x02;
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);
	return(UDR);
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)
	{
		UART_TxChar(str[j]);
		j++;
	}
}
void UART_Transmit(unsigned char data)
{
	UDR = data;
	while ( !( UCSRA & (1<<UDRE) ) );
}

