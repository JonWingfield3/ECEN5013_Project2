/*
 * log.c
 *
 *  Created on: Mar 1, 2017
 *      Author: jonathanwingfield
 */

#include "log.h"

CircBuf TXBuf, RXBuf;

log_error log_data(uint8_t* data, uint32_t length){


	uart_write_byte_n(data,length);
}

log_error log_string(uint8_t* string){

	uint32_t i;
	while(*string)// != '\0')
		uart_write_byte(string++);
}

log_error log_integer(uint8_t num){


}

log_error log_flush(void){


}
