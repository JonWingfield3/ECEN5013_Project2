/*
 * log.c
 *
 *  Created on: Mar 1, 2017
 *      Author: jonathanwingfield
 */

#include "log.h"

extern CircBuf TXBuf;

log_error log_data(uint8_t* data, uint32_t length){

#ifdef FRDM
	uart_send_byte_n(data,length);
#endif
#ifndef FRDM
	uint8_t i = 0;
	for(i = 0; i < length; ++i){
		printf("%c", data[i]);
	}
#endif

}

log_error log_string(uint8_t* string){
#ifdef FRDM
	while(*string != '\0')
		uart_send_byte(string++);
#endif
#ifndef FRDM
	printf("%s", string);
#endif
}

log_error log_integer(int32_t num){

	int8_t my_str[11];
	my_itoa(my_str, num, 10);
#ifdef FRDM
	log_string(my_str);
#endif
#ifndef FRDM
	printf("%s", my_str);
#endif
}

log_error log_flush(void){

	while(BufferEmpty(&TXBuf) == BUFFER_EMPTY);
}
