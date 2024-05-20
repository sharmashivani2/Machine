

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include  "tcp.h"
// Atmega32 SPI I/O

// Define W5100 Socket Register and Variables Used
uint8_t sockreg;
#define MAX_BUF 512
uint8_t buf[MAX_BUF];
int tempvalue;
uint8_t ledmode,ledeye,ledsign;
void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	DDRB|=(1<<PB3);  /*set OC0 pin as output*/
}
void uart_init(void)
{
	UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8;		// set baud rate
	UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
	UCSRB = (1<<RXEN)|(1<<TXEN); 				// enable Rx & Tx
	UCSRC=  (1<<UCSZ1)|(1<<UCSZ0);  	        // config USART; 8N1
}
void uart_flush(void)
{
	unsigned char dummy;
	while (UCSRA & (1<<RXC)) dummy = UDR;
}
int uart_putch(char ch,FILE *stream)
{
	if (ch == '\n')
	uart_putch('\r', stream);
	while (!(UCSRA & (1<<UDRE)));
	UDR=ch;
	return 0;
}
int uart_getch(FILE *stream)
{
	unsigned char ch;
	while (!(UCSRA & (1<<RXC)));
	ch=UDR;

	/* Echo the Output Back to terminal */
	uart_putch(ch,stream);

	return ch;
}
void ansi_cl(void)
{
	// ANSI clear screen: cl=\E[H\E[J
	putchar(27);
	putchar('[');
	putchar('H');
	putchar(27);
	putchar('[');
	putchar('J');
}
void ansi_me(void)
{
	// ANSI turn off all attribute: me=\E[0m
	putchar(27);
	putchar('[');
	putchar('0');
	putchar('m');
}






ISR(TIMER0_OVF_vect)
{
	static unsigned char tenms=1;
	tenms++;                  // Read ADC every 20 x 10ms = 200 milisecond
	if (tenms >= 20) {
		cli();                                // Disable Interupt
		// Select the LED Mode here
		if (ledmode == 1) {
			if (ledeye <= 0) ledeye=0x01;
			if (ledsign == 0) {
				PORTD=ledeye;
				ledeye=ledeye << 1;
				if (ledeye >= 0x80) ledsign=1;
				} else {
				PORTD=ledeye;
				ledeye=ledeye >> 1;
				if (ledeye <= 0x01) ledsign=0;
			}
			} else {
			if (ledsign == 0) {
				PORTD=0x00;
				ledsign=1;
				} else {
				PORTD=0xFF;
				ledsign=0;
			}
		}
		// Set ADMUX Channel for LM35DZ Input
		ADMUX=0x01;
		// Start conversion by setting ADSC on ADCSRA Register
		ADCSRA |= (1<<ADSC);
		// wait until convertion complete ADSC=0 -> Complete
		while (ADCSRA & (1<<ADSC));
		// Get the ADC Result
		tempvalue = ADCW;
		// ADC = (Vin x 1024) / Vref, Vref = 1 Volt, LM35DZ Out = 10mv/C
		tempvalue = (int)(tempvalue) / 10.24;

		tenms=1;

		sei();                            // Enable Interupt
	}
	// Start counter from 0x94, overflow at 10 mSec
	TCNT0=0x94;
}

// Assign I/O stream to UART
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

int main(void){
	uint8_t sockstat;
	uint16_t rsize;
	 PWM_init();
	OCR0= 155;
	DDRC=0xFF;
	char radiostat0[10],radiostat1[10],temp[4],radiostat2[10],radiostat3[10];
	int getidx,postidx;
	//DDRA = (1 << Relay);


	// Reset Port D
	DDRD = 0xFF;       // Set PORTD as Output
	PORTD = 0x00;

	//#if _DEBUG_MODE
	// Define Output/Input Stream
	stdout = stdin = &uart_str;
	// Initial UART Peripheral
	//uart_init();
	UART_init();
	// Clear Screen
	ansi_me();
	ansi_cl();
	ansi_me();
	ansi_cl();
	uart_flush();
	//#endif
	// Initial ATMega386 ADC Peripheral
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	// Free running ADC Mode
	
	// Initial the AVR ATMega328 SPI Peripheral
	// Set MOSI (PORTB3),SCK (PORTB5) and PORTB2 (SS) as output, others as input
	SPI_DDR = (1<<PORTB4)|(1<<PORTB5)|(1<<PORTB7);
	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
	// Enable SPI, Master Mode 0, set the clock rate fck/2
	SPCR = (1<<SPE)|(1<<MSTR);
	SPSR |= (1<<SPI2X);

	// Initial ATMega368 Timer/Counter0 Peripheral
	TCCR0=0x00;                  // Normal Timer0 Operation
	TCCR0=(1<<CS02)|(1<<CS00);   // Use maximum prescaller: Clk/1024
	TCNT0=0x94;                   // Start counter from 0x94, overflow at 10 mSec
	TIMSK=(1<<TOIE0);            // Enable Counter Overflow Interrupt
	sei();                        // Enable Interrupt
	
	// Initial the W5100 Ethernet
	W5100_Init();
	// Initial variable used

	//UART_SendString("web server ");
	sockreg=0;
	tempvalue=0;
	ledmode=1;
	ledeye=0x01;                  // Initial LED Eye Variables
	ledsign=0;
	//#if _DEBUG_MODE
	//printf("WEB Server Debug Mode\n\n");
	//#endif

	// Loop forever
	for(;;){
		
		sockstat=SPI_Read(S0_SR);
		
		
		//printf("hello%d",sockstat);
		switch(sockstat) {
			case SOCK_CLOSED:
			if (socket(sockreg,MR_TCP,TCP_PORT) > 0)
			{
				// Listen to Socket 0
				if (listen(sockreg) <= 0)
				_delay_ms(1);
				//#if _DEBUG_MODE
				printf("Socket Listen!\n");
				//#endif
			}
			break;
			
			
			case SOCK_ESTABLISHED:
			// Get the client request size
			//printf("waiting for connection");
			rsize=recv_size();
			//#if _DEBUG_MODE
			
			//printf("data %d",buf);
			//#endif
			//	start:
			
			if (rsize > 0) {
				// Now read the client Request
				printf("Size: %d\n",rsize);
				if (recv(sockreg,buf,rsize) <= 0) break;
				//#if _DEBUG_MODE
				
				
				
				printf("Content:\n%s\n",buf);
				// Check the Request Header
				getidx=strindex((char *)buf,"GET /");
				postidx=strindex((char *)buf,"POST /");

				if (getidx >= 0 || postidx >= 0) {
					//#if _DEBUG_MODE
					printf("Req. Check!\n");
					//#endif
					// Now check the Radio Button for POST request
					if (postidx >= 0) {
						if (strindex((char *)buf,"radio=0") > 0)
						ledmode=0;
						if (strindex((char *)buf,"radio=1") > 0)
						ledmode=1;
					}
					#if _DEBUG_MODE
					printf("Req. Send!\n");
					#endif
					
					// Now Send the HTTP Response
					if (send(sockreg,buf,strlen((char *)buf)) <= 0) break;

					

					if (ledmode == 1) {
						strcpy(radiostat0,"");
						strcpy_P(radiostat1,PSTR("checked"));
						PORTC|=(1<<PC0);

						} else {
						strcpy_P(radiostat0,PSTR("checked"));
						strcpy(radiostat1,"");
						PORTC&=~(0<<PC0);

					}
					
					
					
					if (send(sockreg,buf,strlen((char *)buf)) <= 0) break;
				}
				disconnect(sockreg);
				//UART_SendString(*buf);
				//#endif
				// Wait for request
				break;
				case SOCK_FIN_WAIT:
				case SOCK_CLOSING:
				case SOCK_TIME_WAIT:
				case SOCK_CLOSE_WAIT:
				case SOCK_LAST_ACK:
				//goto start;
				// Force to close the socket
				close(sockreg);
				//#if _DEBUG_MODE
				printf("Socket Close!\n");
				//#endif
				break;
			}
			
			
		}}
		return 0;
	}
	