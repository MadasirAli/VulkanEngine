#pragma once

#include"ctypes.h"
#include"logger.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include<vulkan\vulkan.h>

#define ENABLE_EXTENSIONS
#define ENABLE_DEVICE_EXTENSIONS

#define API_VERSION VK_API_VERSION_1_0

#define ENABLED_LAYERS_COUNT 1
#define ENABLED_EXTENSIONS_COUNT 2
#define ENABLED_DEVICE_EXTENSIONS_COUNT 1

#define ENABLED_LAYERS const char* pEnabledLayers[ENABLED_LAYERS_COUNT] = {"VK_LAYER_KHRONOS_validation"}
#define ENABLED_EXTENSIONS const char* pEnabledExtensions[ENABLED_EXTENSIONS_COUNT] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME}
#define ENABLED_DEVICE_EXTENSIONS const char* pEnabledDeviceExtensions[ENABLED_DEVICE_EXTENSIONS_COUNT] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME}

#define GRAPHICS_QUEUE_PRIORITY 1.0f
#define PRESENTATION_QUEUE_PRIORITY GRAPHICS_QUEUE_PRIORITY

void CreateVulkanInstance(VkInstance* pVulkanInstance);
void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, uint32_t graphicsQueueFamilyIndex, uint32_t presentationQueueFamilyIndex, VkDevice* pVkDevice);
void CreateVulkanSurface(HWND hWnd, VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);
void CreateSwapchain(VkPhysicalDevice* pVkPhysicalDevice, VkDevice* pVkDevice, VkSurfaceKHR* pVkSurfaceKHR, VkSwapchainKHR* pVkSwapChainKHR, VkFormat* pVkFormat, VkExtent2D* pVkExtend2D);
VkImageView* CreateSwapchainImageViews(VkDevice* pVkDevice, VkImage* pVkSwapchainImages, uint32_t numberOfImages, VkFormat* pVkImageFormat);

void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice);
VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDeviceGraphicsQueueFamily(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDevicePresentationQueueFamily(VkPhysicalDevice* pVkPhysicalDevice, VkSurfaceKHR* pVkSurface);
void GetDeviceQueue(VkDevice* pVkDevice, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pVkQueue);
VkImage* GetSwapchainImages(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchainKHR, uint32_t* numberOfImages);

bool CheckDeviceExtensionsAvailability(VkPhysicalDevice* pVkPhysicalDevice);
bool CheckInstanceExtensionsAvailability();

void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice);

void DestroyVulkanSwapchain(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchain);
void DestroyVulkanInstance(VkInstance* pVulkanInstance);
void DestroyVulkanSurface(VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);
void DestroyVulkanDevice(VkDevice* pVkDevice);

void FreeSwapChainImages(VkImage* pVkImageList);
void FreeSwapchainImageViews(VkDevice* pVkDevice, VkImageView* pVkImageViewList, uint32_t numberOfImageViews);