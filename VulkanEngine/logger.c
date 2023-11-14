#include "logger.h"
#include "ctypes.h"

#include<stdio.h>
#include<windows.h>

void log(char* msg)
{
#ifdef _DEBUG
	printf("LOG: %s\n", msg);
#endif
}

void error(char* msg)
{
	printf("ERROR: %s\n", msg);

#ifndef _DEBUG
	ExitProcess(MAXUINT32);
#endif
}