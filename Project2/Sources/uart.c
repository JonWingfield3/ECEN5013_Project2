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
	// Set Baud Rate
	UART0_BDH =
}

/* Baud rate dividers
115200 => 208 + 1/3
38400  => 625 + 0
57600  => 416 + 2/3
*/

UART_RETURN uart_send_byte() {

}

UART_RETURN uart_send_byte_n() {

}

UART_RETURN uart_receive_byte() {

}
