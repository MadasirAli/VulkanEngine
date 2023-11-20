#pragma once

#include"ctypes.h"
#include"logger.h"
#include"vertex.h"

#include"vulkaninternal.h"

void Draw();
void RecreateSwapchain();
void InitializeVulkan(HWND hWnd, char* pApplicationName);

void DestroyVulkan(HWND hWnd);