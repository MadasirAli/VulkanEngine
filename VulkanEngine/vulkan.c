#include "vulkan.h"

VkInstance _vulkanInstance = NULL;
VkPhysicalDevice _physicalDevice = NULL;
VkDevice _logicalDevice = NULL;
VkQueue _graphicsQueue = NULL;
VkQueue _presentationQueue = NULL;

VkSurfaceKHR _vulkanSurface = NULL;

VkSwapchainKHR _swapChain = NULL;
VkImage* _pVkSwapchainImageList = NULL;
uint32_t _numberOfSwapchainImages = 0;
VkFormat _swapChainImageFormat = 0;
VkExtent2D _swapChainExtend2D = { 0 };

VkImageView* _pSwapChainImageViewsList = NULL;

VkRenderPass  _renderPass = NULL;
VkPipelineLayout _pipelineLayout = NULL;
VkPipeline _pipeLine = NULL;

VkFramebuffer* _pFramebufferList = NULL;

VkCommandPool _commandPool = NULL;
VkCommandBuffer _commandBuffer = NULL;

VkSemaphore _imageAvailableSemaphore = NULL;
VkSemaphore _imageRenderedSemaphore = NULL;
VkFence _waitForRenderFence = NULL;

VkBuffer _vertexBuffer = NULL;
VkDeviceMemory _vertexBufferMemory = NULL;
VkDeviceSize _vertexBufferMemorySize = 0;

bool _vulkanInitialized = false;
bool _swapchainRecreationRequired = false;

vertex _triangleVertices[3] = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

void Draw()
{
	if (_vulkanInitialized == false)
	{
		error("Vulkan is not Initialized Yet.");
		return;
	}

	if (_swapchainRecreationRequired == true)
	{
		RecreateSwapchain();

		_swapchainRecreationRequired = false;
		return;
	}

	DrawFrame(&_logicalDevice, &_pipeLine, &_swapChain, &_renderPass, &_commandBuffer, _pFramebufferList, &_graphicsQueue, &_presentationQueue, &_swapChainExtend2D, &_waitForRenderFence, &_imageAvailableSemaphore, &_imageRenderedSemaphore, &_swapchainRecreationRequired,
		&_vertexBuffer);
}

void RecreateSwapchain()
{
	vkDeviceWaitIdle(_logicalDevice);

	DestroyAndFreeFramebuffers(&_logicalDevice, _pFramebufferList, _numberOfSwapchainImages);

	DestroyAndFreeSwapchainImageViews(&_logicalDevice, _pSwapChainImageViewsList, _numberOfSwapchainImages);
	FreeSwapChainImages(_pVkSwapchainImageList);
	DestroyVulkanSwapchain(&_logicalDevice, &_swapChain);


	CreateSwapchain(&_physicalDevice, &_logicalDevice, &_vulkanSurface, &_swapChain, &_swapChainImageFormat, &_swapChainExtend2D);
	_pVkSwapchainImageList = GetSwapchainImages(&_logicalDevice, &_swapChain, &_numberOfSwapchainImages);
	_pSwapChainImageViewsList = CreateSwapchainImageViews(&_logicalDevice, _pVkSwapchainImageList, _numberOfSwapchainImages, &_swapChainImageFormat);

	_pFramebufferList = CreateFramebuffers(&_logicalDevice, &_renderPass, &_swapChainExtend2D, _pSwapChainImageViewsList, _numberOfSwapchainImages);
}

void InitializeVulkan(HWND hWnd, char* pApplicationName)
{
	CreateVulkanInstance(&_vulkanInstance, pApplicationName);
	CreateVulkanSurface(hWnd, &_vulkanSurface, &_vulkanInstance);

	GetPhysicalDevice(&_vulkanInstance, &_physicalDevice);

	if (CheckDeviceExtensionsAvailability(&_physicalDevice) == false)
	{
		error("Device Does not support the required Device Extensions.");
		return;
	}

	optional graphicsQueueFamilyIndex = { 0 };
	graphicsQueueFamilyIndex = GetPhysicalDeviceGraphicsQueueFamily(&_physicalDevice);
	if (graphicsQueueFamilyIndex.hasValue == false)
	{
		error("GRAPHICS_BIT Queue Family not Found on Selected Device.");
		return;
	}

	optional presentationQueueFamilyIndex = { 0 };
	presentationQueueFamilyIndex = GetPhysicalDevicePresentationQueueFamily(&_physicalDevice, &_vulkanSurface);
	if (presentationQueueFamilyIndex.hasValue == false)
	{
		error("Selected Device Does not Support Presentation Queue Family.");
		return;
	}

	VkPhysicalDeviceFeatures physicalDeviceFeatures = { 0 };
	physicalDeviceFeatures = GetPhysicalDeviceFeatures(&_physicalDevice);

	CreateLogicalDevice(&_physicalDevice, &physicalDeviceFeatures, graphicsQueueFamilyIndex.value, presentationQueueFamilyIndex.value, &_logicalDevice);

	GetDeviceQueue(&_logicalDevice, graphicsQueueFamilyIndex.value, 0, &_graphicsQueue);
	GetDeviceQueue(&_logicalDevice, presentationQueueFamilyIndex.value, 0, &_presentationQueue);

	CreateSwapchain(&_physicalDevice, &_logicalDevice, &_vulkanSurface, &_swapChain, &_swapChainImageFormat, &_swapChainExtend2D);
	_pVkSwapchainImageList = GetSwapchainImages(&_logicalDevice, &_swapChain, &_numberOfSwapchainImages);
	_pSwapChainImageViewsList = CreateSwapchainImageViews(&_logicalDevice, _pVkSwapchainImageList, _numberOfSwapchainImages, &_swapChainImageFormat);

	CreateVertexBuffer(&_physicalDevice, &_logicalDevice, 3, &_vertexBuffer, &_vertexBufferMemory, &_vertexBufferMemorySize);

	// filling vertex buffer
	void* vertexBufferMemory = MapMemory(&_logicalDevice, &_vertexBufferMemory, _vertexBufferMemorySize);
	if (memcpy(vertexBufferMemory, _triangleVertices, (size_t)_vertexBufferMemorySize) == NULL)
	{
		error("Failed to Fill Vertex Buffer.");
		return;
	}
	UnMapMemory(&_logicalDevice, &_vertexBufferMemory);
	//

	CreateRenderPass(&_logicalDevice, &_swapChainImageFormat, &_renderPass);
	CreatePipelineLayout(&_logicalDevice, &_pipelineLayout);
	CreatePipeline(&_logicalDevice, &_pipelineLayout, &_renderPass, &_swapChainExtend2D, &_pipeLine);

	_pFramebufferList = CreateFramebuffers(&_logicalDevice, &_renderPass, &_swapChainExtend2D, _pSwapChainImageViewsList, _numberOfSwapchainImages);

	CreateCommandPool(&_logicalDevice, &_commandPool, graphicsQueueFamilyIndex.value);
	CreateCommandBuffer(&_logicalDevice, &_commandPool, &_commandBuffer);

	CreateVulkanSemaphore(&_logicalDevice, &_imageAvailableSemaphore);
	CreateVulkanSemaphore(&_logicalDevice, &_imageRenderedSemaphore);
	CreateVulkanFence(&_logicalDevice, &_waitForRenderFence, true);

	_vulkanInitialized = true;
}

void DestroyVulkan(HWND hWnd)
{
	vkDeviceWaitIdle(_logicalDevice);

	DestroyVulkanFence(&_logicalDevice, &_waitForRenderFence);
	DestroyVulkanSemaphore(&_logicalDevice, &_imageRenderedSemaphore);
	DestroyVulkanSemaphore(&_logicalDevice, &_imageAvailableSemaphore);
	DestroyCommandPool(&_logicalDevice, &_commandPool);
	DestroyAndFreeFramebuffers(&_logicalDevice, _pFramebufferList, _numberOfSwapchainImages);
	DestroyBuffer(&_logicalDevice, &_vertexBuffer, &_vertexBufferMemory);
	DestroyPipeline(&_logicalDevice, &_pipeLine);
	DestroyRenderPass(&_logicalDevice, &_renderPass);
	DestroyPipelineLayout(&_logicalDevice, &_pipelineLayout);
	DestroyAndFreeSwapchainImageViews(&_logicalDevice, _pSwapChainImageViewsList, _numberOfSwapchainImages);
	FreeSwapChainImages(_pVkSwapchainImageList);
	DestroyVulkanSwapchain(&_logicalDevice, &_swapChain);
	DestroyVulkanDevice(&_logicalDevice);
	DestroyVulkanSurface(&_vulkanSurface, &_vulkanInstance);
	DestroyWindow(hWnd);
	DestroyVulkanInstance(&_vulkanInstance);
}