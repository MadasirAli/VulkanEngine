#include "logger.h"
#include "ctypes.h"

#include<stdio.h>
#include<windows.h>

void log(char* msg)
{
#ifdef _DEBUG
	printf("\033[1;33mLOG: %s\n\033[1;0m", msg);
#endif
}

void logf(char* format, char* msg)
{
#ifdef _DEBUG
	printf(format, msg);
#endif
}

void error(char* msg)
{
	printf("\033[1;31mERROR: %s\n\033[1;0m", msg);

#ifndef _DEBUG
	ExitProcess(MAXUINT32);
#endif
}