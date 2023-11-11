#include "logger.h"

#include<stdio.h>

void log(char* msg)
{
#ifdef _DEBUG
	printf("LOG: %s\n", msg);
#endif
}

void error(char* msg)
{
	printf("ERROR: %s\n", msg);
}