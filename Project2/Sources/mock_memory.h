#ifndef __MOCK_MEMORY_H
#define __MOCK_MEMORY_H

#include <cmocka.h>
#include <stdint.h>
#include "defines.h"

int8_t __wrap_my_memmove(uint8_t* src, uint8_t* dst, uint32_t length);
int8_t __wrap_my_memset(uint8_t* src, uint32_t length, uint8_t value);
int8_t __wrap_my_memzero(uint8_t* src, uint32_t length);
int8_t __wrap_my_reverse(uint8_t* src, uint32_t length);


#endif
