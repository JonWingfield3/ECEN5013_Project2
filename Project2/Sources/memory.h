#ifndef __MEMORY_H__
#define __MEMORY_H__

// standard includes
#include <stdint.h>

#define PTR_ERROR  ((int8_t)-1)
#define SUCCESS    ((int8_t)0)

/***********************************************************************************
* 	File: memory.h
* 	
*	Dependencies: stdint.h, memory.c
*	
*	Description: Contains prototypes for 4 different functions used to manipulate
*	memory. Implementations of the functions is in memory.c. Detailed descriptions
*	of each function given below.
*
***********************************************************************************/


/***********************************************************************************
* int8_t my_memmove(uint8_t* src, uint8_t* dst, uint32_t length)
*	
*	Description: Copies length number of bytes from memory location pointed
*		to by src to memory location pointed to by dst. Works properly for
*		case where pointers overlap, that is |src - dest| < length. Neither
*		src nor dst should be NULL, function returns PTR_ERROR(-1) if either
*		pointer is NULL. Returns SUCCESS(0) after successfully moving all bytes. 
*
*	Parameters: 
*		- uint8_t* src: Pointer which holds memory address of source data. 
*		- uint8_t* dst: Pointer which holds memory address of destination data.
*		- uint32_t length: Value which indicates number of bytes to be transferred
*		from source to destination.
*
*	Return value: int8_t. Function will return PTR_ERROR if either src or dest is 
*		a NULL pointer. Returns SUCCESS if neither pointer is NULL and length bytes 
*		are copied.
*		
************************************************************************************/
int8_t my_memmove(uint8_t* src, uint8_t* dst, uint32_t length);



/***********************************************************************************
*	int8_t my_memset(uint8_t* src, uint32_t length, uint8_t value)
*	
*	Description: Sets chunk of memory equal to value. The size of the chunk of memory 
* 		is determined by the value of length. The base memory address is determined by 
*		src. Function return value indicates SUCCESS/FAILURE of operation.
*
*	Parameters: 
*		- uint8_t* src: Pointer which holds base memory address of memory to be set.. 
*		- uint32_t length: Value which indicates number of bytes to be written into 
*		  with value.
*		- uint8_t value: Holds the value that each byte of memory will be assigned to.		
*
*	Return value: int8_t. Function will return PTR_ERROR if src is a NULL pointer. 
*		Returns SUCCESS after length bytes are written into with value.
*		
*************************************************************************************/
int8_t my_memset(uint8_t* src, uint32_t length, uint8_t value);



/************************************************************************************
*int8_t my_memzero(uint8_t* src, uint32_t length)
*
*	Description: Sets chunk of memory equal to 0. The size of the chunk of memory 
* 		is determined by the value of length. The base memory address is determined by 
*		src. Function return value indicates SUCCESS/FAILURE of operation.
*
*	Parameters: 
*		- uint8_t* src: Pointer which holds base memory address of memory to be set. 
*		- uint32_t length: Value which indicates number of bytes to be zeroed out..
*
*	Return value: int8_t. Function will return PTR_ERROR if src is a NULL pointer. 
*		Returns SUCCESS after length bytes are zeroed out.
*		
*************************************************************************************/
int8_t my_memzero(uint8_t* src, uint32_t length);



/************************************************************************************
*	int8_t my_reverse(uint8_t* src, uint32_t length)
*
*	Description: Reverses chunk of memory of size length where base address is held
*		in src. Checks to make sure src is a valid pointer ( != NULL). After call to 
*		function memory is reversed. That is, *(src + length) --> *src, 
*		*src --> *(src + length) and so on.
*
*	Parameters: 
*		- uint8_t* src: Pointer which holds base memory address of memory to be reversed. 
*		- uint32_t length: Value which indicates number of bytes to be reversed. 
*
*	Return value: int8_t. Function will return PTR_ERROR if src is a NULL pointer. 
*		Returns SUCCESS after length bytes are reversed.
*		
*************************************************************************************/
int8_t my_reverse(uint8_t* src, uint32_t length);


#endif /* __MEMORY_H__ */
