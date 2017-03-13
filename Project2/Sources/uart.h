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
#include "uartbuf.h"

extern uint32_t data_flag;

typedef enum {
	UART_INIT_SUCCESS,
	UART_INIT_FAILURE,
	UART_SEND_SUCCESS,
	UART_SEND_FAILURE,
	UART_SEND_BUFADD_SUCCESS,
	UART_SEND_BUFADD_FAILURE,
	UART_RECEIVE_SUCCESS,
	UART_RECEIVE_FAILURE
}UART_RETURN;


/***************************************************************
 * UART_RETURN uart_configure(void)
 *	Description: Configures UART0 for 57600 BR, 8-N-1 UART comm.
 *	INTERRUPT switch being set initializes UART TX and RX buffers
 *	and enables RX interrupts.
 *	Parameters:
 *		- none
 *	Possible Return Values:
 *		- UART_INIT_SUCCESS: UART succesfully initialized
 *		- UART_INIT_FAILURE: when INTERRUTPS switch set,
 *			occurs if Buffer for UART are unable to be created
 **************************************************************/
UART_RETURN uart_configure(void);


/***************************************************************
 * UART_RETURN uart_send_byte(uint8_t* data)
 *	Description: sends 8 bits of data out of UART0
 *	Parameters:
 *		- uint8_t* data: ptr to byte of data to be sent
 *	Possible Return Values:
 *		- UART_SEND_SUCCESS: data successfully sent
 *		- UART_SEND_FAILURE: data is NULL, no data sent
 *		- UART_SEND_BUFADD_SUCCESS: when INTERRUPTS switch is set,
 *			indicates that data has been buffered.
 *		- UART_SEND_BUFADD_FAILURE: when INTERRUPTS switch is set,
 *			indicates that data was not able to be buffered.
 **************************************************************/
UART_RETURN uart_send_byte(uint8_t* data);


/***************************************************************
 * UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length)
 *	Description: sends n bytes of data out of UART0.
 *	If interrupts are enabled, function blocks until there is room in
 *	in the TXBuf.
 *	Parameters:
 *		- uint8_t* data: pointer to data being sent
 *		- uint32_t length: number of bytes pointed at by data
 *	Possible Return Values: *
 *		- UART_SEND_SUCCESS: data successfully sent
 *		- UART_SEND_FAILURE: data is NULL, no data sent
 *		- UART_SEND_BUFADD_SUCCESS: when INTERRUPTS switch is set,
 *			indicates that data has been buffered.
 *		- UART_SEND_BUFADD_FAILURE: when INTERRUPTS switch is set,
 *			indicates that data was not able to be buffered.
 **************************************************************/
UART_RETURN uart_send_byte_n(uint8_t* data, uint32_t length);


/***************************************************************
 * UART_RETURN uart_receive_byte(uint8_t* buffer)
 *	Description: receives 8 bits of data from UART0 and stores it into buffer. Blocks
 *	until byte is received.
 *	Parameters:
 *		- uint8_t* buffer: pointer to location where data is going to be stored
 *	Possible Return Values:
 *		- UART_RECEIVE_FAILURE: buffer is NULL
 *		- UART_RECEIVE_SUCCESS: data written into buffer
 **************************************************************/
UART_RETURN uart_receive_byte(uint8_t* buffer);


/***************************************************************
 * UART_RETURN uart_receive_byte_n(uint8_t* buffer, uint32_t length)
 *	Description: receives n bytes of data from UART0 and stores into buffer.
 *	This function blocks until length bytes are received.
 *	Parameters:
 *		- uint8_t* buffer: pointer to location where data is stored.
 *		- uint2_t length: number of bytes to be read.
 *	Possible Return Values:
 *		- UART_RECEIVE_FAILURE: buffer is NULL
 *		- UART_RECEIVE_SUCCESS: length bytes received successfully.
 **************************************************************/
UART_RETURN uart_receive_byte_n(uint8_t* data, uint32_t length);

#endif /* SOURCES_UART_H_ */
