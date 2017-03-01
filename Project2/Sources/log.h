/*
 * log.h
 *
 *  Created on: Mar 1, 2017
 *      Author: jonathanwingfield
 */

#include "uart.h"
#include "data.h"
#include "memory.h"
#include "circbuf.h"
#include "defines.h"

#ifndef SOURCES_LOG_H_
#define SOURCES_LOG_H_

typedef enum log_error_t {
	SUCCESS_LOG,
	NOT_SURE_WHICH_OTHER_CONDITIONS_TO_INCLUDE_YET
} log_error;

log_error log_data(uint8_t* data, uint32_t length);

log_error log_string(uint8_t* string);

log_error log_integer(uint8_t num);

log_error log_flush(void);

#endif /* SOURCES_LOG_H_ */
