

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include  "tcp.h"
// Atmega32 SPI I/O
uint16_t relaymode;

// Define W5100 Socket Register and Variables Used
uint8_t sockreg;
#define MAX_BUF 512
uint8_t buf[MAX_BUF];
int tempvalue;
uint8_t ledmode,ledeye,ledsign;
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

	

	DDRA = 0xFF;

	DDRD = 0xFF;

	DDRC = 0xFF;

	static unsigned char tenms=1;

	tenms++; // Read ADC every 20 x 10ms = 200 milisecond

	if (tenms >= 20) {

		cli(); // Disable Interupt

		

		// Select the RelayMode here

		

		switch(relaymode)

		{

			

			case 0:

			// Do nothing

			break;

			

			case 1:

			// relay 1

			PORTA |= 0xFF;

			break;

			

			case 2:

			//realy 2

				PORTB |= 0xFF;

			break;

			

			case 3:

			//realay 3
	PORTC |= 0xFF;
			break;
            case 4:
			PORTA|=0x00;
			break;
			case 5:
			PORTB|=0x00;
			break;
			case 6:
			PORTB|=0x00;
			
			case 7:
			PORTB|=0x00;
			case 8:
			PORTB|=0x00;
			case 9:
			PORTB|=0x00;
			case 10:
			PORTB|=0x00;
			case 11:
			PORTB|=0x00;
			case 12:
			PORTB|=0x00;
			case 13:
			PORTB|=0x00;
			case 14:
			PORTB|=0x00;
			case 15:
			PORTB|=0x00;
			case 16:
			PORTB|=0x00;
			case 17:
			PORTB|=0x00;
			case 18:
			PORTB|=0x00;
			case 19:
			PORTB|=0x00;
			case 20:
			PORTB|=0x00;
			case 21:
			PORTB|=0x00;
			case 22:
			PORTB|=0x00;
			case 23:
			PORTB|=0x00;
			case 24:
			PORTB|=0x00;
			case 25:
			PORTB|=0x00;
			case 26:
			PORTB|=0x00;
			case 27:
			PORTB|=0x00;
			case 28:
			PORTB|=0x00;
			case 29:
			PORTB|=0x00;
			case 30:
			PORTB|=0x00;
			case 31:
			PORTB|=0x00;
			case 32:
			PORTB|=0x00;
			case 33:
			PORTB|=0x00;
			case 34:
			PORTB|=0x00;
			case 35:
			PORTB|=0x00;
			case 36:
			PORTB|=0x00;
			case 37:
			PORTB|=0x00;
			case 38:
			PORTB|=0x00;
			case 39:
			PORTB|=0x00;
			
			case 40:
			PORTB|=0x00;
			case 41:
			PORTB|=0x00;
			case 42:
			PORTB|=0x00;
			case 43:
			PORTB|=0x00;
			

	}		

			
                                  
		}
}
		

// Assign I/O stream to UART
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

int main(void){
	uint8_t sockstat;
	uint16_t rsize;
	char radiostat0[10],radiostat1[10],temp[4],radiostat2[10],radiostat3[10],radiostat4[10],radiostat5[10],radiostat6[10],radiostat7[10],radiostat8[10],radiostat9[10],radiostat10[10],radiostat11[10],radiostat12[10],radiostat13[10],radiostat14[10],radiostat15[10],radiostat16[10],radiostat17[10],radiostat18[10];
	char radiostat19[10],radiostat20[10],radiostat21[10],radiostat22[10],radiostat23[10],radiostat25[10],radiostat26[10],radiostat27[10],radiostat28[10],radiostat29[10],radiostat30[10],radiostat31[10],radiostat32[10],radiostat33[10],radiostat34[10],radiostat35[10],radiostat36[10],radiostat37[10],radiostat38[10];
	char radiostat39[10],radiostat40[10],radiostat41[10],radiostat42[10],radiostat24[10],radiostat43[10];
	int getidx,postidx;
	DDRA = (1 << Relay);


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
	uint16_t relaymode;
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
				getidx=strindex((char *)buf,"GET/");
				postidx=strindex((char *)buf,"POST/");

				if (getidx >= 0 || postidx >= 0) {
					//#if _DEBUG_MODE
					printf("Req. Check!\n");
					//#endif
					if (postidx >= 0) {

						

						if (strindex((char *)buf,"radio=0") > 0)

						relaymode=0;

						

						if (strindex((char *)buf,"radio=1") > 0)

						relaymode=1;

						

						if (strindex((char *)buf,"radio=2") > 0)

						relaymode=2;

						

						if (strindex((char *)buf,"radio=3") > 0)

						relaymode=3;
	if (strindex((char *)buf,"radio=4") > 0)


	     relaymode=4;
		 	if (strindex((char *)buf,"radio=5") > 0)

		 	relaymode=5;
			 	if (strindex((char *)buf,"radio=6") > 0)

			 	relaymode=6;
				 
				 if (strindex((char *)buf,"radio=7") > 0)

				 relaymode=7;
				 
				 if (strindex((char *)buf,"radio=8") > 0)

				 relaymode=8;
				 
				 if (strindex((char *)buf,"radio=9") > 0)

				 relaymode=9;
				 
				 if (strindex((char *)buf,"radio=10") > 0)

				 relaymode=10;
				 
				 if (strindex((char *)buf,"radio=11") > 0)

				 relaymode=11;
				 
				 if (strindex((char *)buf,"radio=12") > 0)

				 relaymode=12;
				 
				 if (strindex((char *)buf,"radio=13") > 0)

				 relaymode=13;
				 
				 if (strindex((char *)buf,"radio=14") > 0)

				 relaymode=14;
				 
				 if (strindex((char *)buf,"radio=15") > 0)

				 relaymode=15;
				 
				 if (strindex((char *)buf,"radio=16") > 0)

				 relaymode=16;
				 
				 if (strindex((char *)buf,"radio=17") > 0)

				 relaymode=17;
				 
				 if (strindex((char *)buf,"radio=18") > 0)

				 relaymode=18;
				 
				 if (strindex((char *)buf,"radio=19") > 0)

				 relaymode=19;
				 
				 if (strindex((char *)buf,"radio=20") > 0)

				 relaymode=20;
				 
				 if (strindex((char *)buf,"radio=21") > 0)

				 relaymode=21;
				 
				 if (strindex((char *)buf,"radio=22") > 0)

				 relaymode=22;
				 
				 if (strindex((char *)buf,"radio=23") > 0)

				 relaymode=23;
				 
				 if (strindex((char *)buf,"radio=24") > 0)

				 relaymode=24;
				 
				 if (strindex((char *)buf,"radio=25") > 0)

				 relaymode=25;
				 
				 if (strindex((char *)buf,"radio=26") > 0)

				 relaymode=26;
				 
				 if (strindex((char *)buf,"radio=27") > 0)

				 relaymode=27;
				 
				 if (strindex((char *)buf,"radio=28") > 0)

				 relaymode=28;
				 
				 if (strindex((char *)buf,"radio=29") > 0)

				 relaymode=29;
				 
				 if (strindex((char *)buf,"radio=30") > 0)

				 relaymode=30;
				 
				 if (strindex((char *)buf,"radio=31") > 0)

				 relaymode=31;
							 if (strindex((char *)buf,"radio=32") > 0)

							 relaymode=32;
							 	 if (strindex((char *)buf,"radio=33") > 0)

							 	 relaymode=33;
								  	 if (strindex((char *)buf,"radio=34") > 0)

								  	 relaymode=34;
									   	 if (strindex((char *)buf,"radio=35") > 0)

									   	 relaymode=35;
												 if (strindex((char *)buf,"radio=36") > 0)

												 relaymode=36;
												 	 if (strindex((char *)buf,"radio=37") > 0)

												 	 relaymode=37;
													  	 if (strindex((char *)buf,"radio=38") > 0)

													  	 relaymode=38;
														   	 if (strindex((char *)buf,"radio=39") > 0)

														   	 relaymode=39;
																	 if (strindex((char *)buf,"radio=40") > 0)

																	 relaymode=40;
																	 	 if (strindex((char *)buf,"radio=41") > 0)

																	 	 relaymode=41;
																		  	 if (strindex((char *)buf,"radio=42") > 0)

																		  	 relaymode=42;
																			   if (strindex((char *)buf,"radio=43") > 0)

																			   relaymode=43;
																			   	

					}

					strcat_P((char *)buf,PSTR("qwert"));

					if (send(sockreg,buf,strlen((char *)buf)) <= 0) break;

					

					strcpy(radiostat0,"");

					strcpy(radiostat1,"");

					strcpy(radiostat2,"");

					strcpy(radiostat3,"");

					strcpy(radiostat4,"");
strcpy(radiostat5,"");
strcpy(radiostat6,"");

strcpy(radiostat7,"");

strcpy(radiostat8,"");

strcpy(radiostat9,"");

strcpy(radiostat10,"");

strcpy(radiostat11,"");
strcpy(radiostat12,"");
strcpy(radiostat13,"");

strcpy(radiostat14,"");

strcpy(radiostat15,"");

strcpy(radiostat16,"");

strcpy(radiostat17,"");

strcpy(radiostat18,"");
strcpy(radiostat19,"");
strcpy(radiostat20,"");

strcpy(radiostat21,"");

strcpy(radiostat22,"");

strcpy(radiostat23,"");

strcpy(radiostat24,"");

strcpy(radiostat25,"");
strcpy(radiostat26,"");
strcpy(radiostat27,"");

strcpy(radiostat28,"");

strcpy(radiostat29,"");

strcpy(radiostat30,"");

strcpy(radiostat31,"");

strcpy(radiostat32,"");
strcpy(radiostat33,"");
strcpy(radiostat34,"");

strcpy(radiostat35,"");

strcpy(radiostat36,"");

strcpy(radiostat37,"");

strcpy(radiostat38,"");

strcpy(radiostat39,"");
strcpy(radiostat40,"");
strcpy(radiostat41,"");


strcpy(radiostat42,"");
strcpy(radiostat43,"");




					

					switch (relaymode)

					{

						case 0:

						strcpy_P(radiostat0,PSTR("checked"));
						strcat_P((char *)buf,PSTR("qwert"));
						UART_Transmit(0xFF);
						UART_Transmit(0x01);
						UART_Transmit(0x00);
						UART_Transmit(0x08);
						UART_Transmit(0x00);
						UART_Transmit(0x1E);
						UART_Transmit(0x27);
						strcat_P((char *)buf,PSTR("qwert"));

						break;

						

						case 1:

						strcpy_P(radiostat1,PSTR("checked"));
						UART_Transmit(0xFF);
						UART_Transmit(0x01);
						UART_Transmit(0x00);
						UART_Transmit(0x10);
						UART_Transmit(0x00);
						UART_Transmit(0x1E);
						UART_Transmit(0x2F);

						break;

						

						case 2:

						//realy 2

						strcpy_P(radiostat2,PSTR("checked"));
UART_Transmit(0xFF);
UART_Transmit(0x01);
UART_Transmit(0x00);
UART_Transmit(0x02);
UART_Transmit(0x1E);
UART_Transmit(0x00);
UART_Transmit(0x21);
						break;

						

						case 3:

						//realay 3

						strcpy_P(radiostat3,PSTR("checked"));
UART_Transmit(0xFF);
UART_Transmit(0x01);
UART_Transmit(0x00);
UART_Transmit(0x04);
UART_Transmit(0x1E);
UART_Transmit(0x00);
UART_Transmit(0x23);
strcat_P((char *)buf,PSTR("qwer"));
						break;
							case 4:

							strcpy_P(radiostat4,PSTR("checked"));
							UART_Transmit(0xFF);
							UART_Transmit(0x01);
							UART_Transmit(0x00);
							UART_Transmit(0x04);
							UART_Transmit(0x1E);
							UART_Transmit(0x00);
							UART_Transmit(0x23);

							break;
								case 5:

								strcpy_P(radiostat5,PSTR("checked"));
								UART_Transmit(0xFF);
								UART_Transmit(0x01);
								UART_Transmit(0x00);
								UART_Transmit(0x04);
								UART_Transmit(0x28);
								UART_Transmit(0x00);
								UART_Transmit(0x2D);

								break;
									case 6:

									strcpy_P(radiostat6,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x06);

									break;
									case 7:

									strcpy_P(radiostat7,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x19);
									UART_Transmit(0x00);
									UART_Transmit(0x1E);

									break;
									case 8:

									strcpy_P(radiostat8,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x23);
									UART_Transmit(0x00);
									UART_Transmit(0x28);

									break;
									case 9:

									strcpy_P(radiostat9,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x14);
									UART_Transmit(0x00);
									UART_Transmit(0x19);


									break;
									case 10:

									strcpy_P(radiostat10,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x0A);
									UART_Transmit(0x00);
									UART_Transmit(0x0F);

									break;
									case 11:

									strcpy_P(radiostat11,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x3C);
									UART_Transmit(0x00);
									UART_Transmit(0x41);

									break;
									case 12:

									strcpy_P(radiostat12,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x3F);
									UART_Transmit(0x00);
									UART_Transmit(0x44);

									break;
									case 13:

									strcpy_P(radiostat13,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);
									UART_Transmit(0x32);
									UART_Transmit(0x00);
									UART_Transmit(0x37);

									break;
									case 14:

									strcpy_P(radiostat14,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x1E);
									UART_Transmit(0x00);
									UART_Transmit(0x21);

									break;
									case 15:

									strcpy_P(radiostat15,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x28);
									UART_Transmit(0x00);
									UART_Transmit(0x2B);

									break;
									case 16:

									strcpy_P(radiostat16,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x04);

									break;
									case 17:

									strcpy_P(radiostat17,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x19);
									UART_Transmit(0x00);
									UART_Transmit(0x1C);

									break;
									case 18:

									strcpy_P(radiostat18,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x23);
									UART_Transmit(0x00);
									UART_Transmit(0x26);

									break;
									case 19:

									strcpy_P(radiostat19,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x14);
									UART_Transmit(0x00);
									UART_Transmit(0x17);

									break;
									case 20:

									strcpy_P(radiostat20,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x0A);
									UART_Transmit(0x00);
									UART_Transmit(0x0D);

									break;
									case 21:

									strcpy_P(radiostat21,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x3C);
									UART_Transmit(0x00);
									UART_Transmit(0x3F);

									break;
									case 22:

									strcpy_P(radiostat22,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x3F);
									UART_Transmit(0x00);
									UART_Transmit(0x42);

									break;
									case 23:

									strcpy_P(radiostat23,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x02);
									UART_Transmit(0x32);
									UART_Transmit(0x00);
									UART_Transmit(0x35);

									break;
									case 24:

									strcpy_P(radiostat24,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x1E);
									UART_Transmit(0x27);

									break;
									case 25:

									strcpy_P(radiostat25,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x28);
									UART_Transmit(0x31);

									break;
									case 26:

									strcpy_P(radiostat26,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x01);
									UART_Transmit(0x0A);

									break;
									case 27:

									strcpy_P(radiostat27,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x19);
									UART_Transmit(0x22);

									break;
									case 28:

									strcpy_P(radiostat28,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x23);
									UART_Transmit(0x2C);

									break;
									case 29:

									strcpy_P(radiostat29,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x14);
									UART_Transmit(0x1D);

									break;
									case 30:

									strcpy_P(radiostat30,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x0A);
									UART_Transmit(0x13);

									break;
									case 31:

									strcpy_P(radiostat31,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x3C);
									UART_Transmit(0x45);

									break;
									case 32:

									strcpy_P(radiostat32,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x3F);
									UART_Transmit(0x48);

									break;
									case 33:

									strcpy_P(radiostat33,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x08);
									UART_Transmit(0x00);
									UART_Transmit(0x32);
									UART_Transmit(0x3B);

									break;
									
									case 34:

									strcpy_P(radiostat34,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x1E);
									UART_Transmit(0x2F);

									break;
									case 35:

									strcpy_P(radiostat35,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x28);
									UART_Transmit(0x39);

									break;
									case 36:

									strcpy_P(radiostat36,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x01);
									UART_Transmit(0x12);

									break;
									case 37:

									strcpy_P(radiostat37,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x19);
									UART_Transmit(0x2A);

									break;
									case 38:

									strcpy_P(radiostat38,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x23);
									UART_Transmit(0x34);

									break;
									case 39:

									strcpy_P(radiostat39,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x14);
									UART_Transmit(0x25);

									break;
									case 40:

									strcpy_P(radiostat40,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x0A);
									UART_Transmit(0x1B);

									break;
									case 41:

									strcpy_P(radiostat41,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x3C);
									UART_Transmit(0x4D);

									break;
									case 42:

									strcpy_P(radiostat40,PSTR("checked"));
									UART_Transmit(0xFF);
									UART_Transmit(0x01);
									UART_Transmit(0x00);
									UART_Transmit(0x10);
									UART_Transmit(0x00);
									UART_Transmit(0x3F);
									UART_Transmit(0x50);
strcat_P((char *)buf,PSTR("qqd "));
case 43:

strcpy_P(radiostat40,PSTR("checked"));
UART_Transmit(0xFF);
UART_Transmit(0x01);
UART_Transmit(0x00);
UART_Transmit(0x10);
UART_Transmit(0x00);
UART_Transmit(0x32);
UART_Transmit(0x43);
strcat_P((char *)buf,PSTR("qqd "));
									break;

						

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
	