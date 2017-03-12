/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdint.h>
#include "MKL25Z4.h"
#include "uart.h"
#include "circbuf.h"
#include "uartbuf.h"
#ifdef DEBUG
#include "defines.h"
#endif
//#ifndef B_LOGGER
#include "log.h"
//#endif
#ifdef B_LOGGER
#include "binary_log.h"
uint32_t data_flag;
#endif
#include "memory.h"
#include "data.h"

void main(void)
{
	//uint8_t data = 0, count = 0;

	#ifdef FRDM
	#ifdef DEBUG
	uart_configure();
	#ifdef B_LOGGER
	CircBuf BLB;
	BinLogBufferInit(&BLB, DEFAULT_BINLOGBUF_SIZE);
	BinLogEvent(&BLB, LOGGER_INITIALIZED, 0, 0);
	BinLogEvent(&BLB,GPIO_INITIALIZED, 0, 0);
	#ifdef INTERRUPTS
	NVIC_EnableIRQ(UART0_IRQn);
	__enable_irq();
	#endif
	BinLogEvent(&BLB, SYSTEM_INITIALIZED, 0, 0);
	#endif
	#endif
	#endif

	while(1){

		#ifdef B_LOGGER
		while(FLAG_IS_CLEAR(data_flag));
		CLEAR_FLAG(data_flag);
		uart_receive_byte(&data);
		count++;

		BinLogEvent(&BLB, DATA_RECEIVED, &data, 1);
		BinLogEvent(&BLB, DATA_ANALYSIS_STARTED, 0 , 0);

		if(IS_ALPHA(data)) BinLogChar(&BLB, DATA_ALPHA_COUNT, data);

		else if(IS_NUMERIC(data)) BinLogChar(&BLB, DATA_NUMERIC_COUNT, data);

		else if(IS_PUNCTUATION(data)) BinLogChar(&BLB, DATA_PUNCTUATION_COUNT, data);

		else BinLogChar(&BLB, DATA_MISC_COUNT, data);

		BinLogEvent(&BLB, DATA_ANALYSIS_COMPLETED, 0, 0);

		if(count == 16){
			count = 0;

			log_string("Alphabetic Characters\n");
			BinLogSendData(&BLB, DATA_ALPHA_COUNT);
			log_string("\n\nPunctuation Characters\n");
			BinLogSendData(&BLB, DATA_PUNCTUATION_COUNT);
			log_string("\n\nNumeric Characters\n");
			BinLogSendData(&BLB, DATA_NUMERIC_COUNT);
			log_string("\n\nMisc Characters\n");
			BinLogSendData(&BLB, DATA_MISC_COUNT);
			BinLogBufferClear(&BLB);
		}

		#endif
		#ifndef B_LOGGER
		/*uart_receive_byte(&data);
		uart_send_byte(&data);*/
		uint8_t data[] = {'a','b','c','\0'};
		        log_string(data);
		        //log_flush();
		        uint8_t i;
		        for(i = 0; i < 4; i++) {
		            *(data + i) = i + 65;
		        }
		        log_data(data, 4);
		      //  log_flush();
		        int32_t ui;
		        for(ui = -10; ui < 11; ui++) {
		            log_integer(ui);
		        }
		       // log_flush();
		#endif
		        while(1);
	}
}
