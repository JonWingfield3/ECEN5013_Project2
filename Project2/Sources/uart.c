/*
 * uart.c
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */

#include "uart.h"

CircBuf TXBuf, RXBuf;

UART_RETURN uart_configure(void) {

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(0);
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);

	UART0_C2  = 0x00;
	UART0_C1  = 0x00;
	UART0_C3  = 0x00;
	UART0_C4  = 0x00;
	UART0_BDH = 0x00;
	UART0_BDL = 0x16;

#ifdef INTERRUPTS
	if(UARTBufferInit(&TXBuf, DEFAULT_UARTBUF_SIZE) != SUCCESS_BUF)
		return UART_INIT_FAILURE;
	if(UARTBufferInit(&RXBuf, DEFAULT_UARTBUF_SIZE) != SUCCESS_BUF)
		return UART_INIT_FAILURE;

	UART0_C2 = UART0_C2_RIE_MASK;

#endif

	UART0_C2 |= UART_C2_RE_MASK | UART0_C2_TE_MASK;
	return UART_INIT_SUCCESS;
}


UART_RETURN uart_send_byte(uint8_t* data) {

#ifdef INTERRUPTS
	while(UARTBufferFull(&TXBuf) == BUFFER_FULL);
	if(UARTBufferAdd(&TXBuf, *data) != SUCCESS_BUF) return UART_SEND_BUFADD_FAILURE;
	UART0_C2 |= UART0_C2_TIE_MASK;
	return UART_SEND_BUFADD_SUCCESS;
#endif

#ifndef INTERRUPTS
	if(data == NULL) return UART_SEND_FAILURE;
	while(!(UART0_S1 & UART_S1_TDRE_MASK)); // wait for transmit buffer to empty
	UART0_D = *data;
	return UART_SEND_SUCCESS;
#endif

}

UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length) {

	uint32_t i;
#ifdef INTERRUPTS
	for(i = 0; i < length; ++i){
		if(UARTBufferAdd(&TXBuf, *(data+i)) != SUCCESS_BUF)
			return UART_SEND_BUFADD_FAILURE;
	}
	if(length) UART0_C2 |= UART0_C2_TIE_MASK;
	return UART_SEND_BUFADD_SUCCESS;
#endif

#ifndef INTERRUPTS
	for(i = 0; i < length; ++i){
		if(uart_send_byte(data + i) == UART_SEND_FAILURE);
		return UART_SEND_FAILURE;
	}
	return UART_SEND_SUCCESS;
#endif

}

UART_RETURN uart_receive_byte(uint8_t* buffer) {

	if(buffer == NULL) return UART_RECEIVE_FAILURE;

#ifdef INTERRUPTS
	while(UARTBufferEmpty(&RXBuf) == BUFFER_EMPTY); // wait for receive buffer to be full
	UARTBufferRemove(&RXBuf, buffer);
#endif
#ifndef INTERRUPTS
	while(!(UART0_S1 & UART_S1_RDRF_MASK)); // wait for receive buffer to be full
	*buffer = UART0_D;
#endif
	return UART_RECEIVE_SUCCESS;
}

UART_RETURN uart_receive_byte_n(uint8_t* buffer, uint32_t length) {

	uint32_t i;
	if(buffer == NULL) return UART_RECEIVE_FAILURE;

	for(i = 0; i < length; ++i){
		uart_receive_byte(buffer + i);
	}
	return UART_RECEIVE_SUCCESS;
}

extern void UART0_IRQHandler(void){

	uint8_t data = 0;

	if(UART0_S1 & UART_S1_TDRE_MASK){
		if(UARTBufferCount(&TXBuf) >= 1){
			UARTBufferRemove(&TXBuf, &data);
			UART0_D = data;
		}
		else UART0_C2 &= ~UART0_C2_TIE_MASK;
	}
	if(UART0_S1 & UART_S1_RDRF_MASK){
		if(UARTBufferFull(&RXBuf) == BUFFER_NOT_FULL){
			data = UART0_D;
			UARTBufferAdd(&RXBuf, data);
			#ifdef B_LOGGER
			SET_FLAG(data_flag);
			#endif
		}
	}
}
