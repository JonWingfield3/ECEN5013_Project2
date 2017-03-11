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

	BinLog** temp_ptr = (BinLog**)CB->head;

	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	if(CB->count == CB->length) return OVERWRITE;

	if(CB->count > 0)
		CB->head = ((BinLog**)CB->head < (BinLog**)CB->buffer + CB->length - 1 ?
				((BinLog**)CB->head) + 1 : CB->buffer);

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
		if(item) *item = *((BinLog**)CB->tail);
		CB->tail = CB->buffer;
		CB->head = CB->buffer;
		CB->count = 0;
		return SUCCESS_BUF;
	}
	if(item) *item = *((BinLog**)CB->tail);
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
			*((BinLog**)(CB->tail - CB->length + n - 1)) : *((BinLog**)(CB->tail + n - 1)));
	return SUCCESS_BUF;
}

CircBufStatus BinLogBufferDestroy(CircBuf* CB){

	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	free(CB->buffer);
	CB->length = 0;
	CB->count = 0;
	CB->head = NULL;
	CB->tail = NULL;
	CB->buffer = NULL;
	return SUCCESS_BUF;
}


