/*
 * uart.h
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */
#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

#include "MKL25Z4.h"
#include "defines.h"
#include "circbuf.h"
typedef enum {
	example
}UART_RETURN;

typedef enum {
	br9600 = 9600,
	br19200 = 19200,
	br38400 = 38400,
	br57200 = 57600,
	br115200 = 115200
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
UART_RETURN uart_configure(STD_BAUD br);

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

/***************************************************************
 * UART_RETURN uart_receive_byte_n(uint8_t* buffer, uint32_t length)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 **************************************************************/
UART_RETURN uart_receive_byte_n(uint8_t* data, uint32_t length);

#endif /* SOURCES_UART_H_ */
