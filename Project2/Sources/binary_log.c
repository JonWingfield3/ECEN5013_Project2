/*
 * binary_log.c
 *
 *  Created on: Mar 8, 2017
 *      Author: jacob
 */

#include "binary_log.h"

CircBufStatus BinLogBufferInit(CircBuf* CB, uint32_t size){

	if(!CB) return PTR_ERROR_BUF;
	if(size == 0) return INIT_FAILURE;
	CB->buffer = (BinLog**)malloc(sizeof(BinLog*) * size);
	if(!(CB->buffer)) return HEAP_FULL;
	CB->head = CB->buffer;
	CB->tail = CB->buffer;
	CB->length = size;
	CB->count = 0;
	return SUCCESS_BUF;
}

CircBufStatus BinLogBufferAdd(CircBuf* CB, BinLog* item){

	BinLog** temp_ptr;

	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	if(CB->count == CB->length) return OVERWRITE;

	if(CB->count > 0)
		CB->head = ((BinLog**)CB->head < (BinLog**)CB->buffer + CB->length - 1 ? ((BinLog**)CB->head) + 1 : CB->buffer);

	temp_ptr = (BinLog**)CB->head;
	*temp_ptr = item;
	(CB->count)++;
	return SUCCESS_BUF;
}


CircBufStatus BinLogBufferRemove(CircBuf* CB, BinLog** item){

	if(!CB || !(CB->buffer) ) return PTR_ERROR_BUF;
	if(CB->count == 0){
		return ITEM_REMOVE_FAILURE;
	}
	if(CB->count == 1){ // return to empty state
		if(item) **item = **(((BinLog**)CB->tail));
		free(*((BinLog**)(CB->tail)));
		CB->tail = CB->buffer;
		CB->head = CB->buffer;
		CB->count = 0;
		return SUCCESS_BUF;
	}
	if(item) **item = **((BinLog**)CB->tail);
	free(*((BinLog**)(CB->tail)));
	CB->tail = (((BinLog**)CB->tail) < ((BinLog**)CB->buffer) + CB->length - 1 ?
			((BinLog**)CB->tail) + 1 : CB->buffer);
	(CB->count)--;
	return SUCCESS_BUF;
}

CircBufStatus BinLogBufferFull(CircBuf* CB){

	if(!CB) return PTR_ERROR_BUF;
	if(CB->count == CB->length) return BUFFER_FULL;
	else return BUFFER_NOT_FULL;
}

CircBufStatus BinLogBufferEmpty(CircBuf* CB){

	if(!CB) return PTR_ERROR_BUF;
	if(CB->count == 0) return BUFFER_EMPTY;
	else return BUFFER_NOT_EMPTY;
}

uint32_t BinLogBufferCount(CircBuf* CB){
	if(!CB) return PTR_ERROR_BUF;
	else return CB->count;
}

CircBufStatus BinLogBufferPeek(CircBuf* CB, BinLog** item_n, uint32_t n){
// returns nth oldest item
	if(!CB || !item_n || !(CB->buffer)) return PTR_ERROR_BUF;
	if(n > CB->count || n < 1) return INVALID_PEEK;

	*item_n = (((BinLog**)CB->tail) + n - 1 > ((BinLog**)CB->buffer) + (CB->length - 1) ?
			*(((BinLog**)CB->tail) - CB->length + n - 1) : *(((BinLog**)CB->tail) + n - 1));
	return SUCCESS_BUF;
}

CircBufStatus BinLogBufferClear(CircBuf* CB){

	uint32_t i;
	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	for(i = 0; i < CB->length; ++i)
		BinLogBufferRemove(CB, NULL);
	return SUCCESS_BUF;
}


CircBufStatus BinLogBufferDestroy(CircBuf* CB){

	uint32_t i;
	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	for(i = 0; i < CB->length; ++i)
		BinLogBufferRemove(CB, NULL);
	free(CB->buffer);
	CB->length = 0;
	CB->count = 0;
	CB->head = NULL;
	CB->tail = NULL;
	CB->buffer = NULL;
	return SUCCESS_BUF;
}

BinLogStatus BinLogCreate(BinLog** BL, BinLogID ID, uint8_t* payload, uint32_t length){

	*BL = (BinLog*) malloc(sizeof(BinLog));
	if(*BL == NULL) return BINLOG_HEAP_FULL;
	(*BL)->ID = ID;
	(*BL)->size = length;
	my_memmove(payload, (*BL)->payload, length);
	return BINLOG_SUCCESS;
}

BinLogStatus BinLogEvent(CircBuf* CB, BinLogID ID, uint8_t* payload, uint32_t length){

	BinLog* BL;
	if(BinLogCreate(&BL, ID, payload, length) == BINLOG_HEAP_FULL);
		return BINLOG_HEAP_FULL;
	if(BinLogBufferAdd(CB, BL) == OVERWRITE) return BINLOGBUF_FULL;
	else BINLOG_SUCCESS;
}

BinLogStatus BinLogChar(CircBuf* CB, BinLogID ID, uint8_t character){

	// search for existing ID
	uint32_t i = 1;
	BinLog* BL = (BinLog*)malloc(sizeof(BinLog));
	if(BL == NULL) return BINLOG_HEAP_FULL;

	while(i <= CB->count){
		BinLogBufferPeek(CB, &BL, i);
		if(BL->ID == ID){
			BL->payload[BL->size % MAX_BINLOG_PAYLOAD_SIZE] = character;
			++(BL->size);
			if(BinLogBufferAdd(CB, BL) == OVERWRITE) return BINLOGBUF_FULL;
			else return BINLOGCHAR_NO_EVENT_CREATED;
		}
		else{
			++i;
		}
	}
	// create new BinLog item if ID doesn't exist in CB
	BinLogEvent(CB, ID, &character, 1);
	return BINLOGCHAR_EVENT_CREATED;
}

BinLogStatus BinLogSendData(CircBuf* CB, BinLogID ID){

	uint32_t i = 1;
	BinLog* BL = (BinLog*)malloc(sizeof(BinLog));
	if(BL == NULL) return BINLOG_HEAP_FULL;

	while(i <= CB->count){
		BinLogBufferPeek(CB, &BL, i);
		if(BL->ID == ID){
			log_string("\nNumber of characters: ");
			log_integer(BL->size);
			if(ID != DATA_MISC_COUNT) {
				log_string("\nCharacters received: ");
				uart_send_byte_n(BL->payload, BL->size);
			}
			free(BL);
			return BINLOG_CHARS_FOUND;
		}
		else{
			++i;
		}
	}
	log_string("\nNumber of characters: 0");
	log_string("\nCharacters received: none");
	free(BL);
	return BINLOG_NO_CHARS_FOUND;
}
