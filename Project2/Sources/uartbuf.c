#include "uartbuf.h"

CircBufStatus UARTBufferInit(CircBuf* CB, uint32_t size){

	if(!CB) return PTR_ERROR_BUF;
	if(size == 0) return INIT_FAILURE;
	CB->buffer = (uint8_t*)malloc(sizeof(uint8_t) * size);
	if(!(CB->buffer)) return HEAP_FULL;
	CB->head = CB->buffer;
	CB->tail = CB->buffer;
	CB->length = size;
	CB->count = 0;
	return SUCCESS_BUF;
}

CircBufStatus UARTBufferAdd(CircBuf* CB, uint8_t item){

	uint8_t* temp_ptr = (uint8_t*)CB->head;

	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	if(CB->count == CB->length) return OVERWRITE;

	if(CB->count > 0)
		CB->head = ((uint8_t*)CB->head < (uint8_t*)CB->buffer + CB->length - 1 ?
				((uint8_t*)CB->head) + 1 : CB->buffer);

	*temp_ptr = item;
	(CB->count)++;
	return SUCCESS_BUF;
}


CircBufStatus UARTBufferRemove(CircBuf* CB, uint8_t* item){

	if(!CB || !(CB->buffer) ) return PTR_ERROR_BUF;
	if(CB->count == 0){
		return ITEM_REMOVE_FAILURE;
	}
	if(CB->count == 1){ // return to empty state
		if(item) *item = *((uint8_t*)CB->tail);
		CB->tail = CB->buffer;
		CB->head = CB->buffer;
		CB->count = 0;
		return SUCCESS_BUF;
	}
	if(item) *item = *((uint8_t*)CB->tail);
	CB->tail = (((uint8_t*)CB->tail) < ((uint8_t*)CB->buffer) + CB->length - 1 ?
			((uint8_t*)CB->tail) + 1 : CB->buffer);
	(CB->count)--;
	return SUCCESS_BUF;
}

CircBufStatus UARTBufferFull(CircBuf* CB){

	if(!CB) return PTR_ERROR_BUF;
	if(CB->count == CB->length) return BUFFER_FULL;
	else return BUFFER_NOT_FULL;
}

CircBufStatus UARTBufferEmpty(CircBuf* CB){

	if(!CB) return PTR_ERROR_BUF;
	if(CB->count == 0) return BUFFER_EMPTY;
	else return BUFFER_NOT_EMPTY;
}

uint32_t UARTBufferCount(CircBuf* CB){
	if(!CB) return PTR_ERROR_BUF;
	else return CB->count;
}

CircBufStatus UARTBufferPeek(CircBuf* CB, uint8_t* item_n, uint32_t n){
// returns nth oldest item
	if(!CB || !item_n || !(CB->buffer)) return PTR_ERROR_BUF;
	if(n > CB->count || n < 1) return INVALID_PEEK;
	*item_n = (((uint8_t*)CB->tail) + n - 1 > ((uint8_t*)CB->buffer) + (CB->length - 1) ?
			*((uint8_t*)(CB->tail - CB->length + n - 1)) : *((uint8_t*)(CB->tail + n - 1)));
	return SUCCESS_BUF;
}

CircBufStatus UARTBufferDestroy(CircBuf* CB){

	if(!CB || !(CB->buffer)) return PTR_ERROR_BUF;
	free(CB->buffer);
	CB->length = 0;
	CB->count = 0;
	CB->head = NULL;
	CB->tail = NULL;
	CB->buffer = NULL;
	return SUCCESS_BUF;
}
