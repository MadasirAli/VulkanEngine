#pragma once

#include"ctypes.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include<vulkan\vulkan.h>

#define ENABLE_EXTENSIONS

#define ENABLED_LAYERS_COUNT 1
#define ENABLED_EXTENSIONS_COUNT 2
#define ENABLED_LAYERS const char* pEnabledLayers[ENABLED_LAYERS_COUNT] = {"VK_LAYER_KHRONOS_validation"}
#define ENABLED_EXTENSIONS const char* pEnabledExtensions[ENABLED_EXTENSIONS_COUNT] = {"VK_KHR_surface", "VK_KHR_win32_surface"}
#define QUEUE_PRIORITY 1.0f

void CreateVulkanInstance(VkInstance* pVulkanInstance);
void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, uint32_t queueFamilyIndex, VkDevice* pVkDevice);
void CreateVulkanSurface(HWND hWnd, VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);

void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice);
VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDeviceQueueFamily(VkPhysicalDevice* pVkPhysicalDevice);
void GetDeviceQueue(VkDevice* pVkDevice, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pVkQueue);

void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice);

void DestroyVulkanInstance(VkInstance* pVulkanInstance);
void DestroyVulkanSurface(VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);
void DestroyVulkanDevice(VkDevice* pVkDevice);