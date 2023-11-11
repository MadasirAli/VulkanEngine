#pragma once

#include"logger.h"
#include<vulkan\vulkan.h>

#include<stdlib.h>



#define ENABLED_LAYERS_COUNT 1
#define ENABLED_EXTENSIONS_COUNT 2
#define ENABLED_LAYERS const char* pEnabledLayers[ENABLED_LAYERS_COUNT] = {"VK_LAYER_KHRONOS_validation"}
#define ENABLED_EXTENSIONS const char* pEnabledExtensions[ENABLED_EXTENSIONS_COUNT] = {"VK_KHR_surface", "VK_KHR_win32_surface"}
#define QUEUE_PRIORITY 1.0f

typedef uint8_t bool;
#define true 1
#define false 0

typedef struct
{
	int value;
	bool hasValue;
} optional;

void CreateVulkanInstance(VkInstance* pVulkanInstance);
void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, optional* pVkQueueFamilyIndex, VkDevice* pVkDevice);

void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice);
VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDeviceQueueFamily(VkPhysicalDevice* pVkPhysicalDevice);

void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice);

void DestroyVulkanInstance(VkInstance* pVulkanInstance);
void DestroyVulkanDevice(VkDevice* pVkDevice);