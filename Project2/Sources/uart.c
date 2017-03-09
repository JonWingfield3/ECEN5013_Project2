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

	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(0);
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);

	UART0_C2  = 0x00;
	UART0_C1  = 0x00;
	UART0_C3  = 0x00;
	UART0_C4  = 0x00;
	UART0_BDH = 0x00;

	// Set Baud Rate
	switch((uint32_t) br)
	{
		case 9600:   UART0_BDL = 130; break;
		case 19200:  UART0_BDL = 65;  break;
		case 38400:
			UART0_C4 = 4;
			UART0_BDL = 104;  break;
		case 57600:  UART0_BDL = 22;  break;
		case 115200: UART0_BDL = 11;  break;
	}

#ifdef INTERRUPTS
	BufferInit(&TXBuf, DEFAULT_BUF_SIZE);
	BufferInit(&RXBuf, DEFAULT_BUF_SIZE);
	UART0_C2 = UART0_C2_RIE_MASK;

#endif

	UART0_C2 |= UART_C2_RE_MASK | UART0_C2_TE_MASK;
}


UART_RETURN uart_send_byte(uint8_t* data) {

#ifdef INTERRUPTS
	BufferAdd(&TXBuf, ((CircBufData_t)*data));
	UART0_C2 |= UART0_C2_TIE_MASK;
#endif
#ifndef INTERRUPTS
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); // wait for transmit buffer to empty
	UART0_D = *data;
	return SUCCESS;
#endif
}

UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length) {

	uint32_t i ;
#ifdef INTERRUPTS
	for(i = 0; i < length; ++i){
		BufferAdd(&TXBuf, ((CircBufData_t)*(data+i)));
	}
	if(length) UART0_C2 |= UART0_C2_TIE_MASK;
#endif
#ifndef INTERRUPTS
	for(i = 0; i < length; ++i){
		uart_send_byte(data + i);
	}
#endif
	return SUCCESS;
}

UART_RETURN uart_receive_byte(uint8_t* buffer) {

#ifdef INTERRUPTS
	while(BufferEmpty(&RXBuf) == BUFFER_EMPTY);
	BufferRemove(&RXBuf, (CircBufData_t*)buffer);
#endif
#ifndef INTERRUPTS
	while(!(UART0_S1 & UART_S1_RDRF_MASK)); // wait for receive buffer to be full
	*buffer = UART0_D;
#endif
	return SUCCESS;
}

UART_RETURN uart_receive_byte_n(uint8_t* buffer, uint32_t length) {

	uint32_t i;
	for(i = 0; i < length; ++i){
		uart_receive_byte(buffer + i);
	}
	return SUCCESS;
}

extern void UART0_IRQHandler(void){

	CircBufData_t data = 0;

	if(UART0_S1 & UART_S1_TDRE_MASK){
		if(BufferCount(&TXBuf) >= 1){
			BufferRemove(&TXBuf, &data);
			UART0_D = data;
		}
		else UART0_C2 &= ~UART0_C2_TIE_MASK;
	}
	if(UART0_S1 & UART_S1_RDRF_MASK){
		if(BufferFull(&RXBuf) == BUFFER_NOT_FULL){
			data = UART0_D;
			BufferAdd(&RXBuf, (CircBufData_t)data);
		}
	}

}


