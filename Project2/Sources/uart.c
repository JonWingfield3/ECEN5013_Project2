/*
 * uart.c
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */

#include "uart.h"

#define ENABLE 1
#define DISABLE 0

UART_RETURN uart_configure(STD_BAUD br, uint8_t channel) {
	UART0_C1 = UART0_C1_DOZEEN(ENABLE); // UART0_C1_ILT(x) Maybe we need this?
	UART0_C2 = UART0_C2_RIE(ENABLE) | UART0_C2_RE(ENABLE) | UART0_C2_TE(ENABLE);
	UART0_BDH = 0x00;

	// Set Baud Rate
	switch((uint32_t)br)
	{
		case 38400:
			UART0_BDH |= 0x02
			UART0_BDL  = 0x71;
		break;
		case 57600:
			UART0_BDH |= 0x01;
			UART0_BDL  = 0xA0; // OSR = 7 would reduce fractional component to 0.0833
			// would still need to adjust BD* values
		break;
		case 115200:
			UART0_BDL = 208; //OSR = 3 would reduce fractional component to 0.0833
			// would still need to adjust BDL value
		break;
	}

	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(0); // see page 196 of TRM
	// above line selects clock source for UART, 3 choices, not sure which


	// STILL NEED TO DEAL WITH FRACTIONAL PARTS.
	// all other UART0 registers seem fine. Not sure if we need to set any other bits
	// need to power on UART?
	// need to make sure clock signal is going to UART
}

/* Baud rate dividers
115200 => 208 + 1/3
38400  => 625 + 0
57600  => 416 + 2/3
*/

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


