/*
 * binary_log.h
 *
 *  Created on: Mar 8, 2017
 *      Author: jacob
 */

#ifndef SOURCES_BINARY_LOG_H_
#define SOURCES_BINARY_LOG_H_

#include <stdlib.h>
#include <stdint.h>
#include "circbuf.h"
#include "defines.h"


// Macro functions obtained using ascii table.
#define IS_ALPHA(X) ((X >= 'A' && X <= 'Z') || (X >= 'a' && X <= 'z'))

#define IS_NUMERIC(X) (X >= '0' && X <= '9')

#define IS_PUNCTUATION(X) ((X >= '!' && X <= ''\'') || \
						   (X >= ':' && X <= '@') || \
						   (X >= '[' && X <= '`') || \
						   (X >= '{' && X <= '~'))

#define IS_CTL(X) ((X >= 0 && X <= 0x20) || (X == 0x7F))

typedef enum BinLogID_e{
	LOGGER_INITIALIZED,
	GPIO_INITIALIZED,
	SYSTEM_INITIALIZED,
	SYSTEM_HALTED,
	INFO,
	WARNING,
	ERROR,
	DATA_RECEIVED,
	DATA_ANALYSIS_STARTED,
	DATA_ALPHA_COUNT,
	DATA_NUMERIC_COUNT,
	DATA_PUNCUATION_COUNT,
	DATA_MISC_COUNT,
	DATA_ANALYSIS_COMPLETED
}BinLogID;

typedef struct BinLog_t{
	BinLogID ID; // identifies packet 2^32 possible values
	uint32_t size; // size of payload
	uint8_t payload[MAX_BINLOG_PAYLOAD_SIZE];
}BinLog;

BinLogStatus BinLog(CircBuf* CB, BinLogID ID, uint8_t* payload, uint32_t length);


/******************************************************
* CircBufStatus BinLogBufferInit(BinLogBuf* CB, uint32_t size)
*	Description: This function is used to initialize
*		a BinLogBuf. The buffer member is set to point
*		at an  array of type BinLog* that can hold size
*		elements. If the heap is full, function returns a
*		HEAP_FULL error. The end result of a successful
* 		call to this function is an empty BinLogBuf.
*	Parameters:
*		- BinLogBuf* CB: This parameter is a pointer to a
*		BinLogBuf. Multiple calls to BufferInit() using
*		the same CB pointer should not be made without calls
*		to BufferDestroy() between them. The function will
*		return a PTR_ERROR if CB is NULL.
*		- uint32_t size: This parameter is the number of elements
*		in the buffer. The value 0 is not allowed and will cause
*		the function to return an INIT_FAILURE error.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer, size is > 0, and
*		heap has enough space to allocate size*sizeof(BinLog*)
*		bytes.
*		- PTR_ERROR: CB is invalid (NULL)
*		- INIT_FAILURE: size is equal to 0
*		- HEAP_FULL: Unable to allocate size*sizeof(BinLog*)
*		bytes from the heap.
******************************************************/
CircBufStatus BinLogBufferInit(CircBuf* LB, uint32_t size);


/******************************************************
* CircBufStatus BinLogBufferAdd(BinLogBuf* CB, BinLogBuf_data_t item)
*	Description: This function is used to add parameter item
*		into an initialized buffer pointed at by CB. This
*		function will by default overwrite the oldest entry
*		if the BinLogBuf is full,
*	Parameters:
*		- BinLogBuf* CB: This parameter should be a valid pointer
*		to a BinLogBuf. After a successful call to this function
*		the BinLogBuf pointed at by CB will now contain a new item.
*		- BinLog* item: This is the value to be added into
*		the BinLogBuf.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to a non-full initialized
*		BinLogBuf, and the item was able to be added.
*		- OVERWRITE: CB is a valid pointer to a full and initialized
*		BinLogBuf and the item has been added by overwriting the previous
*		oldest entry.
*		- PTR_ERROR: CB is a non-valid pointer, or points to a non-initialized
*		BinLogBuf. The item has not been added.
******************************************************/
CircBufStatus BinLogBufferAdd(CircBuf* CB, BinLog* item);


/******************************************************
* CircBufStatus BinLogBufferRemove(BinLogBuf* CB)
*	Description: This function is used to remove the oldest item
*		previously inside of a BinLogBuf pointed at by CB. If successful
*		the item parameter will point at the entry that has just
*		been removed.
*	Parameters:
*		- BinLogBuf* CB: This parameter should be a valid pointer
*		to an initialized, non-empty BinLogBuf.
*		- BinLog** item: This parameter should be a valid
*		pointer which upon successful completion of the function
*		will be pointing at a copy of the removed item. The NULL
*		pointer may be passed in for item if the removed value is
*		of no interest.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to a non-empty initialized
*		BinLogBuf, and the item was able to be removed after being copied
*		into the memory location pointed at by item.
*		- ITEM_REMOVE_FAILURE: CB points at an empty BinLogBuf. Nothing can
*		be removed and the value of *item is the same as before.
*		- PTR_ERROR: CB is an invalid pointer or points to an
*		uninitialized BinLogBuf. The function
*		will return without having done any work.
******************************************************/
CircBufStatus BinLogBufferRemove(CircBuf* CB, BinLog** item);


/******************************************************
* CircBufStatus BinLogBufferFull(BinLogBuf* CB)
*	Description: This function can be used to check if
*		 a BinLogBuf is full.
*	Parameters:
*		BinLogBuf* CB: This parameter should be a valid pointer
*		to an initialized BinLogBuf
*	Possible Return Values:
*		- BUFFER_FULL: CB is a valid pointer to an initialized
*		and full BinLogBuf.
*		- BUFFER_NOT_FULL: CB is a valid pointer to an
*		initialized an non-full BinLogBuf
*		- PTR_ERROR: CB is an invalid pointer, or points
*		to an uninitialized BinLogBuf
******************************************************/
CircBufStatus BinLogBufferFull(CircBuf* CB);


/******************************************************
* CircBufStatus BinLogBufferEmpty(BinLogBuf* CB)
*	Description: This function can be used to check if a
*		BinLogBuf is empty.
*	Parameters:
*		- BinLogBuf* CB: This parameter should be a valid pointer
*		to an initialized BinLogBuf
*	Possible Return Values:
*		- BUFFER_EMPTY: CB is a valid pointer to an initialized
*		and empty BinLogBuf
*		- BUFFER_NOT_EMPTY: CB is a valid pointer to an
*		initialized and non-empty BinLogBuf
		_ PTR_ERROR: CB is an invalid pointer or points at an
*		uninitialized BinLogBuf.
******************************************************/
CircBufStatus BinLogBufferEmpty(CircBuf* CB);


/******************************************************
* CircBufStatus BufferPeek(BinLogBuf* CB, BinLog** item_n, uint32_t n)
*	Description: This function is used to return the nth item
*		inside of a BinLogBuf
*	Parameters:
*		- BinLogBuf* CB: This parameter should be a valid pointer
*		to an initialized BinLogBuf containing at least n items.
*		- BinLog** item_n: This parameter should be a valid
*		pointer that upon successful completion of the function call
*		will point at the nth item in the BinLogBuf.
*		- uint32_t n: This parameter indicates which item should be peeked at.
*		n is one-based, so n = 1 returns the first value in the buffer.
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to an initialized BinLogBuf with at least
*		n items inside of it. item_n will point at a copy of the nth item in the
*		BinLogBuf.
*		- INVALID_PEEK: CB is a valid pointer to an initialized BinLogBuf which contains
*		less than n items or n < 1.
*		- PTR_ERROR: CB is invalid or points at an uninitialized BinLogBuf or item_n
*		is an invalid pointer.
******************************************************/
CircBufStatus BinLogBufferPeek(CircBuf* CB, BinLog** item_n, uint32_t n);


/******************************************************
* CircBufStatus BinLogBufferDestroy(BinLogBuf* CB)
*	Description: This function destroys a BinLogBuf and returns
*		its memory back the heap. To use this buffer again a call
*		to BufferInitialize() must be made.
*	Parameters:
*		- BinLogBuf* CB: This should be a pointer to an initialized
*		BinLogBuf.
*	Possible Return Values:
*		- SUCCESS: The previously valid BinLogBuf pointed at by CB
*		has been destroyed, and its dynamic memory has been returned
*		for later use,
*		- PTR_ERROR: CB is an invalid pointer or points at an uninitialized
*		BinLogBuf. No work is done in this case.
******************************************************/
CircBufStatus BinLogBufferDestroy(CircBuf* CB);


/******************************************************
* CircBufStatus BinLogBufferCount(BinLogBuf* CB)
*	Description: This function returns the number of items in
*		BinLogBuf
*	Parameters:
*		- BinLogBuf* CB: This should be a pointer to an initialized
*		BinLogBuf.
*	Possible Return Values:
*		- <number of items in buffer>: Returns for valid initialized buffer
*		- PTR_ERROR: CB is NULL
******************************************************/
uint32_t BinLogBufferCount(CircBuf* CB);

/******************************************************
 * BinLog* BinLogCreate(BinLogID ID, uint8_t* payload)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 *
******************************************************/
BinLogStatus BinLogCreate(BinLog* BL , BinLogID ID, uint8_t* payload, uint32_t length);


/******************************************************
 * CircBufStatus LogInit(BinLog* BL)
 *	Description:
 *
 *	Parameters:
 *
 *	Possible Return Values:
 *
 *
******************************************************/
BinLogStatus LogSend(BinLog* BL);


#endif /* SOURCES_BINARY_LOG_H_ */
