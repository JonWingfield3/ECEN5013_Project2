/*
 * uart.c
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */

#include "uart.h"


#define ENABLE 1
#define DISABLE 0

static CircBuf TXBuf, RXBuf;

UART_RETURN uart_configure(STD_BAUD br) {

	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // The internet said to use this one
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(0); // scale the clock correctly
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);

	UART0_C2 = 0x00;
	UART0_C1 = 0x00;
	UART0_C3 = 0x00;
	UART0_C4 = 0x00;
	UART0_BDH = 0x00;

	// Set Baud Rate
	switch((uint32_t) br)
	{
		case 9600:  UART0_BDL = 130; break;
		case 19200: UART0_BDL = 65;  break;
		case 38400: UART0_BDL = 32;  break;
		case 57600: UART0_BDL = 21;  break;
		case 115200:UART0_BDL = 11;  break;
	}

#ifdef INTERRUPTS
	BufferInit(&TXBuf, DEFAULT_BUF_SIZE);
	BufferInit(&RXBuf, DEFAULT_BUF_SIZE);
#endif

	UART0_C2 = UART_C2_RE_MASK | UART0_C2_TE_MASK;
}


UART_RETURN uart_send_byte(uint8_t* data) {

	while(!(UART0_S1 & UART_S1_TDRE_MASK)); // wait for transmit buffer to empty
	UART0_D = *data;
	return SUCCESS;
}

UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length) {

	uint32_t i ;
	for(i = 0; i < length; ++i){
		uart_send_byte(data + i);
	}
	return SUCCESS;
}

UART_RETURN uart_receive_byte(uint8_t* buffer) {

	while(!(UART0_S1 & UART_S1_RDRF_MASK)); // wait for receive buffer to be full
	*buffer = UART0_D;
	return SUCCESS;
}

UART_RETURN uart_receive_byte_n(uint8_t* buffer, uint32_t length) {

	uint32_t i ;
	for(i = 0; i < length; ++i){
		uart_receive_byte(buffer + i);
	}
	return SUCCESS;
}


