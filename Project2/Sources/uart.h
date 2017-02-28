/*
 * uart.h
 *
 *  Created on: Feb 28, 2017
 *      Author: jacob
 */

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

UART_RETURN uart_configure(STD_BAUD br, uint8_t channel);

UART_RETURN uart_send_byte();

UART_RETURN uart_send_byte_n();

UART_RETURN uart_receive_byte();



#endif /* SOURCES_UART_H_ */
