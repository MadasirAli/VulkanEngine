#pragma once

#include"ctypes.h"
#include"logger.h"

#include"vulkaninternal.h"

void Draw();
void InitializeVulkan(HWND hWnd, char* pApplicationName);

void DestroyVulkan(HWND hWnd);