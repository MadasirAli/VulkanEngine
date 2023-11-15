#pragma once

#include"ctypes.h"
#include"logger.h"
#include"window.h"
#include"vulkan.h"

#include<stdlib.h>

#define CLOSE_UI_EXIT_CODE 1

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);