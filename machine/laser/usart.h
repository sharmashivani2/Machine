/*
 * usart.h
 *
 * Created: 10/3/2019 5:14:57 PM
 *  Author: trans
 */ 


#ifndef USART_H_
#define USART_H_

void UART_init();
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);
void UART_Transmit(unsigned char data);



#endif /* USART_H_ */