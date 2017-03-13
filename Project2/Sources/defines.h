/*
 * defines.h
 *
 *  Created on: Mar 1, 2017
 *      Author: jacob
 */

#ifndef SOURCES_DEFINES_H_
#define SOURCES_DEFINES_H_

#define PTR_ERROR (int8_t) -1
#define SUCCESS (int8_t) 0

#define INTERRUPTS
#define DEFAULT_UARTBUF_SIZE 256
#define DEFAULT_BINLOGBUF_SIZE 128

#define SET_FLAG(FLAG)      FLAG = 1
#define CLEAR_FLAG(FLAG)    FLAG = 0
#define FLAG_IS_SET(FLAG)   FLAG == 1
#define FLAG_IS_CLEAR(FLAG) FLAG == 0

#define MAX_BINLOG_PAYLOAD_SIZE 16

#define B_LOGGER

#define heartbeat_configure() 	do{ \
									SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; \
									PORTB_PCR18 = PORT_PCR_MUX(1); \
									GPIOB_PDDR |= (1<<18); \
									GPIOB_PDOR &= ~(1<<18); \
								}while(0)

#define heartbeat()             GPIOB_PTOR |= (1<<18)

#endif /* SOURCES_DEFINES_H_ */
