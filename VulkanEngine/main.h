#pragma once
#include<stdlib.h>

#include"logger.h"
#include"ctypes.h"
#include"window.h"

#include"vulkan.h"

#define CLOSE_UI_EXIT_CODE 1

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);