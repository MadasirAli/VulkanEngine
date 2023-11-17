#pragma once

#include"ctypes.h"
#include"logger.h"
#include"file.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include<vulkan\vulkan.h>

#define DYNAMIC_VIEW_PORT

#define COLOR_BLENDING
#define ALPHA_BLENDING

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

#define VERTEX_SHADER_PATH "shaders\\vertex_shader.spv"
#define FRAGMENT_SHADER_PATH "shaders\\fragment_shader.spv"

#define DEFAULT_SHADER_ENTRY_POINT "main"
#define VERTEX_SHADER_ENTRY_POINT DEFAULT_SHADER_ENTRY_POINT
#define FRAGMENT_SHADER_ENTRY_POINT DEFAULT_SHADER_ENTRY_POINT

void Draw();
void InitializeVulkan(HWND hWnd, char* pApplicationName);

void DrawFrame(VkDevice* pVkDevice, VkPipeline* pVkPipeline, VkSwapchainKHR* pVkSwapchainKHR, VkRenderPass* pVkRenderPass, VkCommandBuffer* pVkCommandBuffer, VkFramebuffer* pVkFramebufferList, VkQueue* pVkGraphicsQueue, VkQueue* pVkPresentationQueue, VkExtent2D* pVkSwapchainExtent2D, VkFence* pVkWaitForRenderFence, VkSemaphore* pVkImageAvailableSemaphore, VkSemaphore* pVkImageRenderedSemahore);
void SubmitGraphicsCommandBuffer(VkCommandBuffer* pVkCommandBuffer, VkQueue* pVkGraphicsQueue, VkSemaphore* pVkWaitSemaphore, VkSemaphore* pVkRenderFinishSemaphore, VkFence* pVkWaitForRenderFence);
void SubmitPresentationQueue(VkSwapchainKHR* pVkSwapchainKHR, VkQueue* pVkPresentationQueue, uint32_t* pImageIndex, VkSemaphore* pVkImageRenderedSemaphore);
void RecordDrawCommand(VkCommandBuffer* pVkCommandBuffer, VkPipeline* pVkPipeline, VkRenderPass* pVkRenderPass, VkFramebuffer* pVkFrameBufferList, VkExtent2D* pVkSwapchainExtent2D, uint32_t* pIndexOfSwapchainImage);

void CreateVulkanInstance(VkInstance* pVulkanInstance, char* pApplicationName);
void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, uint32_t graphicsQueueFamilyIndex, uint32_t presentationQueueFamilyIndex, VkDevice* pVkDevice);
void CreateVulkanSurface(HWND hWnd, VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);
void CreateSwapchain(VkPhysicalDevice* pVkPhysicalDevice, VkDevice* pVkDevice, VkSurfaceKHR* pVkSurfaceKHR, VkSwapchainKHR* pVkSwapChainKHR, VkFormat* pVkFormat, VkExtent2D* pVkExtend2D);
VkImageView* CreateSwapchainImageViews(VkDevice* pVkDevice, VkImage* pVkSwapchainImages, uint32_t numberOfImages, VkFormat* pVkImageFormat);
VkShaderModule CreateShaderModule(VkDevice* pVkDevice, char* shaderName, byte** ppShaderBytes);
void CreateRenderPass(VkDevice* pVkDevice, VkFormat* pVkSwapchainImageFormat, VkRenderPass* pVkRenderPass);
void CreatePipelineLayout(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout);
void CreatePipeline(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout, VkRenderPass* pVkRenderPass, VkExtent2D* pVkSwapchainExtent2D, VkPipeline* pVkPipeline);
VkFramebuffer* CreateFramebuffers(VkDevice* pVkDevice, VkRenderPass* pVkRenderPass, VkExtent2D* pVkSwapchainExtend2D, VkImageView* pVkImagesViews, uint32_t numberOfImageViews);
void CreateCommandPool(VkDevice* pVkDevice, VkCommandPool* pVkCommandPool, uint32_t queueFamily);
void CreateCommandBuffer(VkDevice* pVkDevice, VkCommandPool* pVkCommandPool, VkCommandBuffer* pVkCommandBuffer);


void CreateVulkanFence(VkDevice* pVkDevice, VkFence* pVkFence, bool signled);
void CreateVulkanSemaphore(VkDevice* pVkDevice, VkSemaphore* pVkSemaphore);

void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice);
VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDeviceGraphicsQueueFamily(VkPhysicalDevice* pVkPhysicalDevice);
optional GetPhysicalDevicePresentationQueueFamily(VkPhysicalDevice* pVkPhysicalDevice, VkSurfaceKHR* pVkSurface);
void GetDeviceQueue(VkDevice* pVkDevice, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pVkQueue);
VkImage* GetSwapchainImages(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchainKHR, uint32_t* numberOfImages);

bool CheckDeviceExtensionsAvailability(VkPhysicalDevice* pVkPhysicalDevice);
bool CheckInstanceExtensionsAvailability();

void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice);

void ResetCommandBuffer(VkCommandBuffer* pVkCommandBuffer);

void DestroyVulkanSwapchain(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchain);
void DestroyVulkanInstance(VkInstance* pVulkanInstance);
void DestroyVulkanSurface(VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance);
void DestroyVulkanDevice(VkDevice* pVkDevice);
void DestroyRenderPass(VkDevice* pVkDevice, VkRenderPass* pVkRenderPass);
void DestroyPipelineLayout(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout);
void DestroyPipeline(VkDevice* pVkDevice, VkPipeline* pVkPipeline);
void DestroyCommandPool(VkDevice* pVkDevice, VkCommandPool* pVkCommandPool);

void DestroyVulkanFence(VkDevice* pVkDevice, VkFence* pVkFence);
void DestroyVulkanSemaphore(VkDevice* pVkDevice, VkSemaphore* pVkSemaphore);

void DestroyAndFreeShaderModule(VkDevice* pVkDevice, VkShaderModule* pVkShaderModule, byte* pShaderBytes);
void FreeSwapChainImages(VkImage* pVkImageList);
void DestroyAndFreeSwapchainImageViews(VkDevice* pVkDevice, VkImageView* pVkImageViewList, uint32_t numberOfImageViews);
void DestroyAndFreeFramebuffers(VkDevice* pVkDevice, VkFramebuffer* pVkFramebufferList, uint32_t numberOfBuffers);

void DestroyVulkan(HWND hWnd);