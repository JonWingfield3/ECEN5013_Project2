#include "data.h"

#define IS_NUM(X) (0x30 <= X && X <= 0x39)
#define NUM_2_ASCII(X) (X + 0x30)
#define ASCII_2_NUM(X) (X - 0x30)

int8_t* my_itoa(int8_t* str, int32_t data, int32_t base)
{
	const int8_t* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	uint8_t i = 0;
	uint32_t u_data;

	if(!str || base > 36 || base < 2) return NULL;

	if(data < 0 && base == 10)
	{
		*(str + i++) = '-';
		u_data = ((uint32_t)-data);
		if(data == 0)
		{// overflow. not very elegant solution but faster than do-while loop
			*(str + i++) = '2';
			*(str + i++) = '1';
			*(str + i++) = '4';
			*(str + i++) = '7';
			*(str + i++) = '4';
			*(str + i++) = '8';
			*(str + i++) = '3';
			*(str + i++) = '6';
			*(str + i++) = '4';
			*(str + i++) = '8';
			*(str + i++) = '\0';
			return str;
		}
	}
	else u_data = (uint32_t)data;

	do
	{
		*(str + i++) = digits[u_data % base];
		u_data /= base;
	}while(u_data);

	if(str[0] == '-')
		my_reverse(str + 1, i - 1);
	else
		my_reverse(str, i);

	*(str + i) = '\0';
	return str;
}

int32_t my_atoi(int8_t* str)
{
	uint32_t i;
	int8_t sign = 1;
	int32_t value = 0;
	if(!str) return -1;
	for(i = 0; *(str + i) != '\0'; ++i)
	{
		if(IS_NUM(*(str + i)))
		{
			value = 10*value + ASCII_2_NUM(*(str + i));
		}
		else if(!value && *(str + i) == '-') sign = -1;
	}
	return sign*value;
}

int8_t big_to_little32(uint32_t* data, uint32_t length)
{
	uint32_t i;
	if(!data) return PTR_ERROR;

	for(i = 0; i < length; ++i)
		my_reverse((((uint8_t*)data) + i), 4);

	return SUCCESS;
}

int8_t little_to_big32(uint32_t* data, uint32_t length)
{
	uint32_t i;
	if(!data) return PTR_ERROR;

	for(i = 0; i < length; ++i)
		my_reverse((((uint8_t*)data) + i),  4);

	return SUCCESS;
}

void print_memory(uint8_t* start, uint32_t length)
{
	uint32_t i;
	if(!start) return;

	for(i = 0; i < length; ++i)
		printf("%x ", *(start + i));
}
