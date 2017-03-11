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

/******************************************************
* CircBufStatus BinLogBufferInit(CircBuf* BLB, uint32_t size)
*	Description: This function is used to initialize
*		a CircBuf. The buffer member is set to point
*		at an  array of type BinLog* that can hold size
*		elements. If the heap is full, function returns a
*		HEAP_FULL error. The end result of a successful
* 		call to this function is an empty CircBuf.
*	Parameters:
*		- CircBuf* BLB: This parameter is a pointer to a
*		CircBuf. Multiple calls to BufferInit() using
*		the same BLB pointer should not be made without calls
*		to BufferDestroy() between them. The function will
*		return a PTR_ERROR if BLB is NULL.
*		- uint32_t size: This parameter is the number of elements
*		in the buffer. The value 0 is not allowed and will cause
*		the function to return an INIT_FAILURE error.
*	Possible Return Values:
*		- SUCCESS: BLB is a valid pointer, size is > 0, and
*		heap has enough space to allocate size*sizeof(BinLog*)
*		bytes.
*		- PTR_ERROR: BLB is invalid (NULL)
*		- INIT_FAILURE: size is equal to 0
*		- HEAP_FULL: Unable to allocate size*sizeof(BinLog*)
*		bytes from the heap.
******************************************************/
CircBufStatus BinLogBufferInit(CircBuf* LB, uint32_t size);

/******************************************************
* CircBufStatus BinLogBufferAdd(CircBuf* BLB, CircBuf_data_t item)
*	Description: This function is used to add parameter item
*		into an initialized buffer pointed at by BLB. This
*		function will by default overwrite the oldest entry
*		if the CircBuf is full,
*	Parameters:
*		- CircBuf* BLB: This parameter should be a valid pointer
*		to a CircBuf. After a successful call to this function
*		the CircBuf pointed at by BLB will now contain a new item.
*		- BinLog* item: This is the value to be added into
*		the CircBuf.
*	Possible Return Values:
*		- SUCCESS: BLB is a valid pointer to a non-full initialized
*		CircBuf, and the item was able to be added.
*		- OVERWRITE: BLB is a valid pointer to a full and initialized
*		CircBuf and the item has been added by overwriting the previous
*		oldest entry.
*		- PTR_ERROR: BLB is a non-valid pointer, or points to a non-initialized
*		CircBuf. The item has not been added.
******************************************************/
CircBufStatus BinLogBufferAdd(CircBuf* BLB, BinLog* item);

/******************************************************
* CircBufStatus BinLogBufferRemove(CircBuf* BLB)
*	Description: This function is used to remove the oldest item
*		previously inside of a CircBuf pointed at by BLB. If successful
*		the item parameter will point at the entry that has just
*		been removed.
*	Parameters:
*		- CircBuf* BLB: This parameter should be a valid pointer
*		to an initialized, non-empty CircBuf.
*		- BinLog** item: This parameter should be a valid
*		pointer which upon successful completion of the function
*		will be pointing at a copy of the removed item. The NULL
*		pointer may be passed in for item if the removed value is
*		of no interest.
*	Possible Return Values:
*		- SUCCESS: BLB is a valid pointer to a non-empty initialized
*		CircBuf, and the item was able to be removed after being copied
*		into the memory location pointed at by item.
*		- ITEM_REMOVE_FAILURE: BLB points at an empty CircBuf. Nothing can
*		be removed and the value of *item is the same as before.
*		- PTR_ERROR: BLB is an invalid pointer or points to an
*		uninitialized CircBuf. The function
*		will return without having done any work.
******************************************************/
CircBufStatus BinLogBufferRemove(CircBuf* BLB, BinLog** item);

/******************************************************
* CircBufStatus BinLogBufferFull(CircBuf* BLB)
*	Description: This function can be used to check if
*		 a CircBuf is full.
*	Parameters:
*		CircBuf* BLB: This parameter should be a valid pointer
*		to an initialized CircBuf
*	Possible Return Values:
*		- BUFFER_FULL: BLB is a valid pointer to an initialized
*		and full CircBuf.
*		- BUFFER_NOT_FULL: BLB is a valid pointer to an
*		initialized an non-full CircBuf
*		- PTR_ERROR: BLB is an invalid pointer, or points
*		to an uninitialized CircBuf
******************************************************/
CircBufStatus BinLogBufferFull(CircBuf* BLB);

/******************************************************
* CircBufStatus BinLogBufferEmpty(CircBuf* BLB)
*	Description: This function can be used to check if a
*		CircBuf is empty.
*	Parameters:
*		- CircBuf* BLB: This parameter should be a valid pointer
*		to an initialized CircBuf
*	Possible Return Values:
*		- BUFFER_EMPTY: BLB is a valid pointer to an initialized
*		and empty CircBuf
*		- BUFFER_NOT_EMPTY: BLB is a valid pointer to an
*		initialized and non-empty CircBuf
		_ PTR_ERROR: BLB is an invalid pointer or points at an
*		uninitialized CircBuf.
******************************************************/
CircBufStatus BinLogBufferEmpty(CircBuf* BLB);

/******************************************************
* CircBufStatus BufferPeek(CircBuf* BLB, BinLog** item_n, uint32_t n)
*	Description: This function is used to return the nth item
*		inside of a CircBuf
*	Parameters:
*		- CircBuf* BLB: This parameter should be a valid pointer
*		to an initialized CircBuf containing at least n items.
*		- BinLog** item_n: This parameter should be a valid
*		pointer that upon successful completion of the function call
*		will point at the nth item in the CircBuf.
*		- uint32_t n: This parameter indicates which item should be peeked at.
*		n is one-based, so n = 1 returns the first value in the buffer.
*	Possible Return Values:
*		- SUCCESS: BLB is a valid pointer to an initialized CircBuf with at least
*		n items inside of it. item_n will point at a copy of the nth item in the
*		CircBuf.
*		- INVALID_PEEK: BLB is a valid pointer to an initialized CircBuf which contains
*		less than n items or n < 1.
*		- PTR_ERROR: BLB is invalid or points at an uninitialized CircBuf or item_n
*		is an invalid pointer.
******************************************************/
CircBufStatus BinLogBufferPeek(CircBuf* BLB, BinLog** item_n, uint32_t n);

/******************************************************
* CircBufStatus BinLogBufferDestroy(CircBuf* BLB)
*	Description: This function destroys a CircBuf and returns
*		its memory back the heap. To use this buffer again a call
*		to BufferInitialize() must be made.
*	Parameters:
*		- CircBuf* BLB: This should be a pointer to an initialized
*		CircBuf.
*	Possible Return Values:
*		- SUCCESS: The previously valid CircBuf pointed at by BLB
*		has been destroyed, and its dynamic memory has been returned
*		for later use,
*		- PTR_ERROR: BLB is an invalid pointer or points at an uninitialized
*		CircBuf. No work is done in this case.
******************************************************/
CircBufStatus BinLogBufferDestroy(CircBuf* BLB);

/******************************************************
* CircBufStatus BinLogBufferCount(CircBuf* BLB)
*	Description: This function returns the number of items in
*		CircBuf
*	Parameters:
*		- CircBuf* BLB: This should be a pointer to an initialized
*		CircBuf.
*	Possible Return Values:
*		- <number of items in buffer>: Returns for valid initialized buffer
*		- PTR_ERROR: BLB is NULL
******************************************************/
uint32_t BinLogBufferCount(CircBuf* BLB);

CircBufStatus LogInit(BinLog* BL);
// initialize BLP to empty BinLog

CircBufStatus LogItem(BinLog* BL, BinLogID event);
// adds item to BinLog

CircBufStatus LogClear(BinLog* BL);
// empties BinLog

CircBufStatus LogSend(BinLog* BL);
// sends contents of BinLog

uint32_t LogPayloadSize(BinLog* BL);
// returns size of payload of  BinLog



#endif /* SOURCES_BINARY_LOG_H_ */
