#pragma once
typedef unsigned int uint32_t;
typedef uint32_t bool;
#define TRUE 1
#define FALSE 0

typedef struct
{
	uint32_t value;
	bool hasValue;
} optional;