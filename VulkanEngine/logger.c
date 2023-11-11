#include "logger.h"

#include<stdio.h>

void log(char* msg)
{
	printf("LOG: %s\n", msg);
}

void error(char* msg)
{
	printf("ERROR: %s\n", msg);
}