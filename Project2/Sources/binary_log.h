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
#include "memory.h"
#include "uart.h"
#include "uartbuf.h"
#include "log.h"

// Macro functions obtained using ascii table.
#define IS_ALPHA(X) ((X >= 'A' && X <= 'Z') || (X >= 'a' && X <= 'z'))

#define IS_NUMERIC(X) (X >= '0' && X <= '9')

#define IS_PUNCTUATION(X) ((X >= '!' && X <= 0x47) || (X >= ':' && X <= '@') || (X >= '[' && X <= '`') || (X >= '{' && X <= '~'))

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
	DATA_PUNCTUATION_COUNT,
	DATA_MISC_COUNT,
	DATA_ANALYSIS_COMPLETED
}BinLogID;

typedef enum BinLogStatus_e{
	BINLOG_SUCCESS,
	BINLOG_HEAP_FULL,
	BINLOG_PTR_ERROR,
	BINLOGBUF_FULL,
	BINLOGCHAR_EVENT_CREATED,
	BINLOGCHAR_NO_EVENT_CREATED,
	BINLOG_CHARS_FOUND,
	BINLOG_NO_CHARS_FOUND
}BinLogStatus;

typedef struct BinLog_t{
	BinLogID ID;
	uint32_t size;
	uint8_t payload[MAX_BINLOG_PAYLOAD_SIZE];
}BinLog;


/******************************************************
* BinLogStatus BinLogEvent(CircBuf* CB, BinLogID ID, uint8_t* payload, uint32_t length)
*	Description: This function is used to create a BinLog item and initialize it
*		it with an ID and payload given by the function's parameters. The newly created
*		BinLog item is then added into the CircBuf pointed at by CB.
*	Parameters:
*		- CircBuf* CB: This parameter should be a pointer to an initialized
*			CircBuf.
*		- BinLogID ID: This parameter is used to indicate the ID of the
*			BinLog that is to be created.
*		- uint8_t* payload: This parameter is a pointer to the payload
*			that is to be copied into the newly created BinLog. The value 0
*			be passed
*		- uint32_t length: This parameter specifies the number of bytes pointed
*			at by payload
*	Possible Return Values:
*		- BINLOG_SUCCESS: BinLog was able to be created and added to CB
*		- BINLOG_HEAP_FULL: BinLog unable to be created.
*		- BINLOGBUF_FULL: BinLog created but not added to CB.
******************************************************/
BinLogStatus BinLogEvent(CircBuf* CB, BinLogID ID, uint8_t* payload, uint32_t length);


/******************************************************
 * BinLogStatus BinLogCreate(BinLog** BL , BinLogID ID, uint8_t* payload, uint32_t length)
 *	Description: Creates BinLog on heap containing data specified by parameters
 *	Parameters:
 *		- BinLog** BL: points at pointer to newly created BinLog
 *		- BinLogID: specifies which type of BinLog is to be created
 *		- uint8_t* payload: data to be added into new BinLog
 *		- uint32_t length: size of payload. number of bytes pointed at by payload.
 *	Possible Return Values:
 *		- BINLOG_HEAP_FULL: BinLog unable to be created
 *		- BINLOG_SUCCESS: BinLog successfully created
******************************************************/
BinLogStatus BinLogCreate(BinLog** BL , BinLogID ID, uint8_t* payload, uint32_t length);


/******************************************************
* BinLogStatus BinLogChar(CircBuf* CB, BinLogID ID, uint8_t character)
*	Description: This function is used to log character data that is received
*		via UART. It searches for an existing BinLog with an ID matching the ID parameter.
*		If one is found the character is added into that BinLog's payload. If it is not found
*		a new BinLog is made and added to the BinLogBuf.
*	Parameters:
*		- CircBuf* CB: This parameter should be a pointer to an initialized
*			CircBuf.
*		- BinLogID ID: This parameter is used to indicate the ID of the
*			BinLog that is to be added to or created.
*		- uint8_t character: This is the data to be added to the payload
*	Possible Return Values:
*		- BINLOG_EVENT_CREATED: A BinLog with a matching ID was not found,
*		and so a new BinLog was created and added to CB successfully.
*		- BINLOG_HEAP_FULL: BinLog unable to be created.
*		- BINLOGBUF_FULL: BinLog created but not added to CB.
*		- BinLog_NO_EVENT_CREATED: A BinLog with a matching ID was found in CB,
*		character was added to the back of the BinLog's payload.
******************************************************/
BinLogStatus BinLogChar(CircBuf* CB, BinLogID ID, uint8_t character);


/******************************************************
* BinLogStatus BinLogSendData(CircBuf* CB, BinLogID ID)
*	Description: This function is used to send out character data that has been logged
*	in CB. Calls functions from log.h.
*	Parameters:
*		- CircBuf* CB: This parameter should be a pointer to an initialized
*			CircBuf.
*		- BinLogID ID: This parameter is used to indicate the ID of the desired
*			character type. ID = DATA_ALPHA_COUNT prints alphabetic characters received,
*			ID = DATA_PUNCTUATION_COUNT prints punctuation characters and count,
*			ID = DATA_NUMERIC_COUNT prints all numerical characters received and count,
*			ID = DATA_MISC_COUNT prints the count of control characters received
*	Possible Return Values:
*		- BINLOG_HEAP_FULL: Space unable to be allocated for function.
*		- BINLOG_CHARS_FOUND: Found matching ID in CB and data of that type is sent out
*			along with its count (except for MISC, only count sent)
*		- BINLOG_NO_CHARS_FOUND: No BinLog with a matching ID was found. String printed
*		out to let user know that no data could be found matching that type.
******************************************************/
BinLogStatus BinLogSendData(CircBuf* CB, BinLogID ID);


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
* CircBufStatus BinLogBufferClear(CircBuf* CB)
*	Description: This function is used to empty the contents
*		of CB. The dynamic memory is returned to the heap.
*	Parameters:
*		- BinLogBuf* CB: This parameter should be a valid pointer
*		to an initialized BinLogBuf. After a call to this function CB
*		will return to its state immediately after
*	Possible Return Values:
*		- SUCCESS: CB is a valid pointer to an initialized BinLogBuf with at least
*		n items inside of it. item_n will point at a copy of the nth item in the
*		BinLogBuf.
*		- INVALID_PEEK: CB is a valid pointer to an initialized BinLogBuf which contains
*		less than n items or n < 1.
*		- PTR_ERROR: CB is invalid or points at an uninitialized BinLogBuf or item_n
*		is an invalid pointer.
******************************************************/
CircBufStatus BinLogBufferClear(CircBuf* CB);


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

#endif /* SOURCES_BINARY_LOG_H_ */
