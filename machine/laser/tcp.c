/*
 * tcp.c
 *
 * Created: 1/1/2020 11:32:09 AM
 *  Author: trans
 */ 
/*
 * tcp.c
 *
 * Created: 12/16/2019 10:38:34 AM
 *  Author: trans
 */ 

#include "tcp.h"
void SPI_Write(uint16_t addr,uint8_t data)
{
	// Activate the CS pin
	SPI_PORT &= ~(1<<SPI_CS);
	// Start Wiznet W5100 Write OpCode transmission
	SPDR = WIZNET_WRITE_OPCODE;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	// Start Wiznet W5100 Address High Bytes transmission
	SPDR = (addr & 0xFF00) >> 8;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	// Start Wiznet W5100 Address Low Bytes transmission
	SPDR = addr & 0x00FF;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));

	// Start Data transmission
	SPDR = data;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
}

unsigned char SPI_Read(uint16_t addr)
{
	// Activate the CS pin
	SPI_PORT &= ~(1<<SPI_CS);
	// Start Wiznet W5100 Read OpCode transmission
	SPDR = WIZNET_READ_OPCODE;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	// Start Wiznet W5100 Address High Bytes transmission
	SPDR = (addr & 0xFF00) >> 8;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	// Start Wiznet W5100 Address Low Bytes transmission
	SPDR = addr & 0x00FF;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));

	// Send Dummy transmission for reading the data
	SPDR = 0x00;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));

	// CS pin is not active
	SPI_PORT |= (1<<SPI_CS);
	return(SPDR);
}
void W5100_Init(void)
{
	// Ethernet Setup
	unsigned char mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
	unsigned char ip_addr[] = {192,168,10,160};
	unsigned char sub_mask[] = {255,255,255,0};
	unsigned char gtw_addr[] = {192,168,10,1};

	// Setting the Wiznet W5100 Mode Register: 0x0000
	SPI_Write(MR,0x80);            // MR = 0b10000000;
	_delay_ms(1);
	// printf("Reading MR: %d\n\n",SPI_Read(MR));
	//UART_SendString(*SPI_Read(MR));
	// Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
	printf("Setting Gateway Address %d.%d.%d.%d\n",gtw_addr[0],gtw_addr[1],\
	gtw_addr[2],gtw_addr[3]);
	SPI_Write(GAR + 0,gtw_addr[0]);
	SPI_Write(GAR + 1,gtw_addr[1]);
	SPI_Write(GAR + 2,gtw_addr[2]);
	SPI_Write(GAR + 3,gtw_addr[3]);
	_delay_ms(1);

	printf("Reading GAR: %d.%d.%d.%d\n\n",SPI_Read(GAR + 0),SPI_Read(GAR + 1),\
	SPI_Read(GAR + 2),SPI_Read(GAR + 3));

	// Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
	printf("Setting Source Address %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",mac_addr[0],mac_addr[1],\
	mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	SPI_Write(SAR + 0,mac_addr[0]);
	SPI_Write(SAR + 1,mac_addr[1]);
	SPI_Write(SAR + 2,mac_addr[2]);
	SPI_Write(SAR + 3,mac_addr[3]);
	SPI_Write(SAR + 4,mac_addr[4]);
	SPI_Write(SAR + 5,mac_addr[5]);
	_delay_ms(1);

	printf("Reading SAR: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n\n",SPI_Read(SAR + 0),SPI_Read(SAR + 1),\
	SPI_Read(SAR + 2),SPI_Read(SAR + 3),SPI_Read(SAR + 4),SPI_Read(SAR + 5));

	// Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
	printf("Setting Sub Mask Address %d.%d.%d.%d\n",sub_mask[0],sub_mask[1],\
	sub_mask[2],sub_mask[3]);
	SPI_Write(SUBR + 0,sub_mask[0]);
	SPI_Write(SUBR + 1,sub_mask[1]);
	SPI_Write(SUBR + 2,sub_mask[2]);
	SPI_Write(SUBR + 3,sub_mask[3]);
	_delay_ms(1);

	printf("Reading SUBR: %d.%d.%d.%d\n\n",SPI_Read(SUBR + 0),SPI_Read(SUBR + 1),\
	SPI_Read(SUBR + 2),SPI_Read(SUBR + 3));

	// Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
	printf("Setting IP Address %d.%d.%d.%d\n",ip_addr[0],ip_addr[1],\
	ip_addr[2],ip_addr[3]);
	SPI_Write(SIPR + 0,ip_addr[0]);
	SPI_Write(SIPR + 1,ip_addr[1]);
	SPI_Write(SIPR + 2,ip_addr[2]);
	SPI_Write(SIPR + 3,ip_addr[3]);
	_delay_ms(1);

	printf("Reading SIPR: %d.%d.%d.%d\n\n",SPI_Read(SIPR + 0),SPI_Read(SIPR + 1),\
	SPI_Read(SIPR + 2),SPI_Read(SIPR + 3));

	// Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
	printf("Setting Wiznet RMSR and TMSR\n\n");
	SPI_Write(RMSR,0x55);
	SPI_Write(TMSR,0x55);

	printf("Done Wiznet W5100 Initialized!\n");

	}

void close(uint8_t sock)
{
	if (sock != 0) return;

	// Send Close Command
	SPI_Write(S0_CR,CR_CLOSE);
	// Waiting until the S0_CR is clear
	while(SPI_Read(S0_CR));
}
void disconnect(uint8_t sock)
{
	if (sock != 0) return;
	// Send Disconnect Command
	SPI_Write(S0_CR,CR_DISCON);
	// Wait for Disconnecting Process
	while(SPI_Read(S0_CR));
}
uint8_t socket(uint8_t sock,uint8_t eth_protocol,uint16_t tcp_port)
{
	uint8_t retval=0;
	if (sock != 0) return retval;

	// Make sure we close the socket first
	if (SPI_Read(S0_SR) == SOCK_CLOSED) {
		close(sock);
	}
	// Assigned Socket 0 Mode Register
	SPI_Write(S0_MR,eth_protocol);

	// Now open the Socket 0
	SPI_Write(S0_PORT,((tcp_port & 0xFF00) >> 8 ));
	SPI_Write(S0_PORT + 1,(tcp_port & 0x00FF));
	SPI_Write(S0_CR,CR_OPEN);                   // Open Socket
	// Wait for Opening Process
	while(SPI_Read(S0_CR));
	// Check for Init Status
	if (SPI_Read(S0_SR) == SOCK_INIT)
	retval=1;
	else
	close(sock);

	return retval;
}

uint8_t listen(uint8_t sock)
{
	uint8_t retval = 0;
	if (sock != 0) return retval;
	if (SPI_Read(S0_SR) == SOCK_INIT) {
		// Send the LISTEN Command
		SPI_Write(S0_CR,CR_LISTEN);

		// Wait for Listening Process
		while(SPI_Read(S0_CR));
		// Check for Listen Status
		if (SPI_Read(S0_SR) == SOCK_LISTEN)
		retval=1;
		else
		close(sock);
	}
	return retval;
}
uint16_t send(uint8_t sock,const uint8_t *buf,uint16_t buflen)
{
	uint16_t ptr,offaddr,realaddr,txsize,timeout;

	if (buflen <= 0 || sock != 0) return 0;
	#if _DEBUG_MODE
	printf("Send Size: %d\n",buflen);
	#endif
	// Make sure the TX Free Size Register is available
	txsize=SPI_Read(SO_TX_FSR);
	txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
	#if _DEBUG_MODE
	printf("TX Free Size: %d\n",txsize);
	#endif
	timeout=0;
	while (txsize < buflen) {
		_delay_ms(1);
		txsize=SPI_Read(SO_TX_FSR);
		txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
		// Timeout for approx 1000 ms
		if (timeout++ > 1000) {
			#if _DEBUG_MODE
			printf("TX Free Size Error!\n");
			#endif
			// Disconnect the connection
			disconnect(sock);
			return 0;
		}
	}

	// Read the Tx Write Pointer
	ptr = SPI_Read(S0_TX_WR);
	offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_TX_WR + 1));
	#if _DEBUG_MODE
	printf("TX Buffer: %x\n",offaddr);
	#endif

	while(buflen) {
		buflen--;
		// Calculate the real W5100 physical Tx Buffer Address
		realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
		// Copy the application data to the W5100 Tx Buffer
		SPI_Write(realaddr,*buf);
		offaddr++;
		buf++;
	}

	// Increase the S0_TX_WR value, so it point to the next transmit
	SPI_Write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );
	SPI_Write(S0_TX_WR + 1,(offaddr & 0x00FF));

	// Now Send the SEND command
	SPI_Write(S0_CR,CR_SEND);

	// Wait for Sending Process
	while(SPI_Read(S0_CR));

	return 1;
}
uint16_t recv(uint8_t sock,uint8_t *buf,uint16_t buflen)
{
	uint16_t ptr,offaddr,realaddr;

	if (buflen <= 0 || sock != 0) return 1;

	// If the request size > MAX_BUF,just truncate it
	if (buflen > MAX_BUF)
	buflen=MAX_BUF - 2;
	// Read the Rx Read Pointer
	ptr = SPI_Read(S0_RX_RD);
	offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_RX_RD + 1));
	#if _DEBUG_MODE
	printf("RX Buffer: %x\n",offaddr);
	#endif

	while(buflen) {
		buflen--;
		realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
		*buf = SPI_Read(realaddr);
		offaddr++;
		buf++;
	}
	*buf='\0';        // String terminated character

	// Increase the S0_RX_RD value, so it point to the next receive
	SPI_Write(S0_RX_RD,(offaddr & 0xFF00) >> 8 );
	SPI_Write(S0_RX_RD + 1,(offaddr & 0x00FF));

	// Now Send the RECV command
	SPI_Write(S0_CR,CR_RECV);
	_delay_us(5);    // Wait for Receive Process

	return 1;
}
uint16_t recv_size(void)
{
	return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}
int strindex(char *s,char *t)
{
	uint16_t i,n;

	n=strlen(t);
	for(i=0;*(s+i); i++) {
		if (strncmp(s+i,t,n) == 0)
		return i;
	}
	return -1;
}