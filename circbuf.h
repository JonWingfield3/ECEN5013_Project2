#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t CircBufData_t

typedef struct CircBuf_S{
	CircBufData_t* buffer;
	CircBufData_t* head;
	CircBufData_t* tail;
	uint32_t length;
	uint32_t count;
}CircBuf;

typedef enum CircBufState_E{
	SUCCESS,
	INIT_FAILURE,
	HEAP_FULL,
	INVALID_PEEK,
	BUFFER_DESTROY_FAILURE,
	ITEM_ADD_FAILURE,
	BUFFER_FULL,
	BUFFER_NOT_FULL,
	BUFFER_EMPTY,
	BUFF_NOT_EMPTY,
	ITEM_REMOVE_FAILURE,
}CircBufState;

/******************************************************
* CircBufState BufferInit(CircBuf* CB, uint32_t size)
*	Description: This function is used to initialize
*		a Circbuf. The buffer member is set to point
*		at an  array of type CircBufData_t that can hold size
*		elements. If the heap is full, function returns a 
*		HEAP_FULL error. The end result of a successful
* 		call to this function is an empty CircBuf.
*	Parameters:
*		- CircBuf* CB: This parameter is a pointer to a newly
*		created CircBuf. Multiple calls to BufferInit() using 
*		the same CB pointer should not be made without calls
*		to BufferDestroy() between them. The function will
*		return a PTR_ERROR if CB is NULL.
*		- uint32_t size: This parameter is the number of elements
*		in the buffer. The value 0 is not allowed and will cause
*		the function to return an INIT_FAILURE error. 
*	Possible Rrturn Values:
*		- SUCCESS: CB is a valid pointer, size is > 0, and
*		heap has enough space to allocate size*sizeof(CircBufData_t)
*		bytes. 
*		- PTR_ERROR: CB is invalid (NULL)
*		- INIT_FAILURE: size is equal to 0
*		- HEAP_FULL: Unable to allocate size*sizeof(CircBufData_t)
*		bytes from the heap. 
******************************************************/
CircBufState BufferInit(CircBuf* CB, uint32_t size);

/******************************************************
* CircBufState BufferAdd(CircBuf* CB, circbuf_data_t item);
*	Description: This function is used to add parameter item
*		into an initiaiized buffer pointed at by CB. This
*		function will by default 
*		
*
******************************************************/
CircBufState BufferAdd(CircBuf* CB, circbuf_data_t item);

/******************************************************
*
*
******************************************************/
CircBufState BufferRemove(Circbuf* CB);

/******************************************************
*
*
******************************************************/
CircBufState BufferFull(Circbuf* CB);

/******************************************************
*
*
******************************************************/
CircBufState BufferEmpty(CircBuf* CB);

/******************************************************
*
*
******************************************************/
CircBufState BufferPeek(CircBuf* CB, CircBufData_t* item_n, uint32_t n);

/******************************************************
*
*
******************************************************/
CircBufState BufferDestroy(CircBuf* CB);

#endif /* __CIRCBUF_H__ */
