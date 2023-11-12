#pragma once

#include<windows.h>

#include"logger.h"
#include"definitions.h"

BOOL GetAndDispatchWindowMessage(HWND hWnd, MSG* messageContainer);
HWND CreateWindowInstance(wchar_t* windowName, WNDPROC wndProc);
