/*
 * uart.h
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */
#include "MKL25Z4.h"

#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

typedef enum {
	example
}UART_RETURN;

typedef enum {
	br115200 = 115200,
	br38400 = 38400,
	br57200 = 57600
}STD_BAUD;

/***************************************************************
 * UART_RETURN uart_configure(STD_BAUD br, uint8_t channel)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 **************************************************************/
UART_RETURN uart_configure(STD_BAUD br, uint8_t channel);

/***************************************************************
 * UART_RETURN uart_send_byte(uint8_t* data)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 **************************************************************/
UART_RETURN uart_send_byte(uint8_t* data);

/***************************************************************
 * UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 **************************************************************/
UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length);

/***************************************************************
 * UART_RETURN uart_receive_byte(uint8_t* buffer)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 **************************************************************/
UART_RETURN uart_receive_byte(uint8_t* buffer);

#endif /* SOURCES_UART_H_ */
