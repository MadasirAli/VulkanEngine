#pragma once

#include<windows.h>

#include"ctypes.h"
#include"logger.h"

BOOL GetAndDispatchWindowMessage(HWND hWnd, MSG* messageContainer);
HWND CreateWindowInstance(wchar_t* windowName, WNDPROC wndProc);

void DestroyWindowInstance(HWND hWnd);
