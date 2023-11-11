#pragma once

#include"logger.h"
#include<vulkan\vulkan.h>

#include<stdlib.h>

#define ENABLED_LAYERS_COUNT 1
#define ENABLED_EXTENSIONS_COUNT 2
#define ENABLED_LAYERS const char* enabledLayers[ENABLED_LAYERS_COUNT] = {"VK_LAYER_KHRONOS_validation"}
#define ENABLED_EXTENSIONS const char* enabledExtensions[ENABLED_EXTENSIONS_COUNT] = {"VK_KHR_surface", "VK_KHR_win32_surface"}

void CreateVulkanInstance(VkInstance* pVulkanInstance);
void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice);
void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice);
void DestroyVulkanInstance(VkInstance* pVulkanInstance);