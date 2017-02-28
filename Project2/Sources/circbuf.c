#include "circbuf.h"

CircBufState BufferInit(CircBuf* CB, uint32_t size){
	
	if(!CB) return PTR_ERROR;
	if(size == 0) return INIT_FAILURE;
	CB->buffer = (CircBufData_t*)malloc(sizeof(CircBufData_t) * size);
	if(!(CB->buffer)) return HEAP_FULL;
	CB->head = CB->buffer;
	CB->tail = CB->buffer;
	CB->length = size;
	CB->count = 0;
	return SUCCESS;	
}

CircBufState BufferAdd(CircBuf* CB, CircBufData_t item){
	
	if(!CB || !(CB->buffer)) return PTR_ERROR;
	if(CB->count == 0){ // handle empty buffer case
		*(CB->head) = item;
		(CB->count)++;
		return SUCCESS;
	}
	if(CB->count <= CB->length){
		CB->head = (CB->head < (CB->buffer + CB->length) ? CB->head + 1 : CB->buffer);
		*(CB->head) = item;
		if(CB->count == CB->length){  // handle full buffer (overwrite)
			return OVERWRITE;
		}
		else{  // handle not full not empty buffer
			(CB->count)++;
		 	return SUCCESS;
		}	
	}	
}

CircBufState BufferRemove(Circbuf* CB, CircBufData_t* item){

	if(!CB || !(CB->buffer) ) return PTR_ERROR; 
	if(CB->count == 0){
		return ITEM_REMOVE_FAILURE;	
	}
	if(CB->count == 1){ // return to empty state
		if(item) *item = *(CB->tail);
		CB->tail = CB->buffer;
		CB->head = CB->buffer;
		CB->count = 0;
		return SUCCESS;
	}	
	if(item) *item = *(CB->tail);
	CB->tail = (CB->tail > CB->buffer ? CB->tail - 1 : CB->tail + CB->length);	
	(CB->count)--;
	return SUCCESS;
}

CircBufState BufferFull(Circbuf* CB){

	if(!CB) return PTR_ERROR;
	if(CB->count == CB->length) return BUFFER_FULL;	
	else return BUFFER_NOT_FULL;
}

CircBufState BufferEmpty(CircBuf* CB){

	if(!CB) return PTR_ERROR;
	if(CB->count == 0) return BUFFER_EMPTY;
	else return BUFFER_NOT_EMPTY;
}

CircBufState BufferPeek(CircBuf* CB, CircBufData_t* item_n, uint32_t n){
// returns nth oldest item
	if(!CB || !item_n || !(CB->buffer)) return PTR_ERROR;
	if(n > CB->count || n < 1) return INVALID_PEEK;
	*item_n = (CB->tail + (n - 1) > CB->buffer + CB->length ? 
		CB->tail + (n -1) - CB->length : CB->tail + (n - 1));
	return SUCCESS;
}

CircBufState BufferDestroy(CircBuf* CB){

	if(!CB || !(CB->buffer)) return PTR_ERROR;
	free(CB->buffer);
	CB->length = 0;
	CB->count = 0;
	CB->head = NULL;
	CB->tail = NULL;
	CB->buffer = NULL;
	return SUCCESS;
}
