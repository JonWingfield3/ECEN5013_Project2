/*
 * circbuf.h
 *
 *  Created on: Mar 10, 2017
 *      Author: jonathanwingfield
 */

#ifndef CIRCBUF_H_
#define CIRCBUF_H_

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t CircBufData_t;

typedef struct CircBuf_t{
	void* buffer;
	void* head;
	void* tail;
	uint32_t length;
	uint32_t count;
}CircBuf;

typedef enum CircBufStatus_e{
	SUCCESS_BUF,
	INIT_FAILURE,
	HEAP_FULL,
	INVALID_PEEK,
	BUFFER_DESTROY_FAILURE,
	ITEM_ADD_FAILURE,
	BUFFER_FULL,
	BUFFER_NOT_FULL,
	BUFFER_EMPTY,
	BUFFER_NOT_EMPTY,
	ITEM_REMOVE_FAILURE,
	OVERWRITE,
	PTR_ERROR_BUF
}CircBufStatus;

/******************************************************
* CircBufStatus BufferInit(CircBuf* CB, uint32_t size)
*	Description: This function is used to initialize
*		a CircBuf. The buffer member is set to point
*		at an  array of type CircBufData_t that can hold size
*		elements. If the heap is full, function returns a
*		HEAP_FULL error. The end result of a successful
* 		call to this function is an empty CircBuf.
*	Parameters:
*		- CircBuf* CB: This parameter is a pointer to a
*		CircBuf. Multiple calls to BufferInit() using
*		the same CB pointer should not be made without calls
*		to BufferDestroy() between them. The function will
*		return a PTR_ERROR if CB is NULL.
*		- uint32_t size: This parameter is the number of elements
*		in the buffer. The value 0 is not allowed and will cause
*		the function to return an INIT_FAILURE error.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer, size is > 0, and
*		heap has enough space to allocate size*sizeof(CircBufData_t)
*		bytes.
*		- PTR_ERROR: CB is invalid (NULL)
*		- INIT_FAILURE: size is equal to 0
*		- HEAP_FULL: Unable to allocate size*sizeof(CircBufData_t)
*		bytes from the heap.
******************************************************/
CircBufStatus BufferInit(CircBuf* CB, uint32_t size);

/******************************************************
* CircBufStatus BufferAdd(CircBuf* CB, CircBuf_data_t item)
*	Description: This function is used to add parameter item
*		into an initialized buffer pointed at by CB. This
*		function will by default overwrite the oldest entry
*		if the CircBuf is full,
*	Parameters:
*		- CircBuf* CB: This parameter should be a valid pointer
*		to a CircBuf. After a successful call to this function
*		the CircBuf pointed at by CB will now contain a new item.
*		- CircBufData_t item: This is the value to be added into
*		the CircBuf.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to a non-full initialized
*		CircBuf, and the item was able to be added.
*		- OVERWRITE: CB is a valid pointer to a full and initalized
*		CircBuf and the item has been added by overwriting the previous
*		oldest entry.
*		- PTR_ERROR: CB is a non-valid pointer, or points to a non-initialized
*		CircBuf. The item has not been added.
******************************************************/
CircBufStatus BufferAdd(CircBuf* CB, CircBufData_t item);

/******************************************************
* CircBufStatus BufferRemove(CircBuf* CB)
*	Description: This function is used to remove the oldest item
*		previously inside of a CircBuf pointed at by CB. If successful
*		the item parameter will point at the entry that has just
*		been removed.
*	Parameters:
*		- CircBuf* CB: This parameter should be a valid pointer
*		to an initialized, non-empty CircBuf.
*		- CircBufData_t* item: This parameter should be a valid
*		pointer which upon successful completion of the function
*		will be pointing at a copy of the removed item. The NULL
*		pointer may be passed in for item if the removed value is
*		of no interest.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to a non-empty initialized
*		CircBuf, and the item was able to be removed after being copied
*		into the memory location pointed at by item.
*		- ITEM_REMOVE_FAILURE: CB points at an empty CircBuf. Nothing can
*		be removed and the value of *item is the same as before.
*		- PTR_ERROR: CB is an invalid pointer or points to an
*		uninitialized CircBuf. The function
*		will return without having done any work.
******************************************************/
CircBufStatus BufferRemove(CircBuf* CB, CircBufData_t* item);

/******************************************************
* CircBufStatus BufferFull(CircBuf* CB)
*	Description: This function can be used to check if
*		 a CircBuf is full.
*	Parameters:
*		CircBuf* CB: This parameter should be a valid pointer
*		to an initialized CircBuf
*	Possible Return Values:
*		- BUFFER_FULL: CB is a valid pointer to an initialized
*		and full CircBuf.
*		- BUFFER_NOT_FULL: CB is a valid pointer to an
*		initialized an non-full CircBuf
*		- PTR_ERROR: CB is an invalid pointer, or points
*		to an uninitialized CircBuf
******************************************************/
CircBufStatus BufferFull(CircBuf* CB);

/******************************************************
* CircBufStatus BufferEmpty(CircBuf* CB)
*	Description: This function can be used to check if a
*		CircBuf is empty.
*	Parameters:
*		- CircBuf* CB: This parameter should be a valid pointer
*		to an initialized CircBuf
*	Possible Return Values:
*		- BUFFER_EMPTY: CB is a valid pointer to an initialized
*		and empty CircBuf
*		- BUFFER_NOT_EMPTY: CB is a valid pointer to an
*		initialized and non-empty CircBuf
		_ PTR_ERROR: CB is an invalid pointer or points at an
*		uninitialized CircBuf.
******************************************************/
CircBufStatus BufferEmpty(CircBuf* CB);

/******************************************************
* CircBufStatus BufferPeek(CircBuf* CB, CircBufData_t* item_n, uint32_t n)
*	Description: This function is used to return the nth item
*		inside of a CircBuf
*	Parameters:
*		- CircBuf* CB: This parameter should be a valid pointer
*		to an initialized CircBuf containing at least n items.
*		- CircBufData_t* item_n: This parameter should be a valid
*		pointer that upon successful completion of the function call
*		will point at the nth item in the CircBuf.
*		- uint32_t n: This parameter indicates which item should be peeked at.
*		n is one-based, so n = 1 returns the first value in the buffer.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to an initialized CircBuf with at least
*		n items inside of it. item_n will point at a copy of the nth item in the
*		CircBuf.
*		- INVALID_PEEK: CB is a valid pointer to an initialized CircBuf which contains
*		less than n items or n < 1.
*		- PTR_ERROR: CB is invalid or points at an uninitialized CircBuf or item_n
*		is an invalid pointer.
******************************************************/
CircBufStatus BufferPeek(CircBuf* CB, CircBufData_t* item_n, uint32_t n);

/******************************************************
* CircBufStatus BufferDestroy(CircBuf* CB)
*	Description: This function destroys a CircBuf and returns
*		its memory back the heap. To use this buffer again a call
*		to BufferInitialize() must be made.
*	Parameters:
*		- CircBuf* CB: This should be a pointer to an initialized
*		CircBuf.
*	Possible Return Values:
*		- SUCCESS: The previously valid CircBuf pointed at by CB
*		has been destroyed, and its dynamic memory has been returned
*		for later use,
*		- PTR_ERROR: CB is an invalid pointer or points at an uninitialized
*		CircBuf. No work is done in this case.
******************************************************/
CircBufStatus BufferDestroy(CircBuf* CB);

/******************************************************
* CircBufStatus BufferCount(CircBuf* CB)
*	Description: This function returns the number of items in
*		CircBuf
*	Parameters:
*		- CircBuf* CB: This should be a pointer to an initialized
*		CircBuf.
*	Possible Return Values:
*		- <number of items in buffer>: Returns for valid initialized buffer
*		- PTR_ERROR: CB is NULL
******************************************************/
uint32_t BufferCount(CircBuf* CB);

#endif /* CIRCBUF_H_ */
