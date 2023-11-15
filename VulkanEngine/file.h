#pragma once

#include"ctypes.h"
#include"logger.h"

#include<stdio.h>
#include<stdlib.h>

byte* ReadBinary(char* fileName, uint32_t* pNumberOfBytes);

void CleanBuffer(byte* buffer);