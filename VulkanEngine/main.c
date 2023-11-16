#include "main.h"

int main()
{
	int32_t exitCode = 0;

	VkInstance vulkanInstance = NULL;
	VkPhysicalDevice physicalDevice = NULL;
	VkDevice logicalDevice = NULL;
	VkQueue graphicsQueue = NULL;
	VkQueue presentationQueue = NULL;

	HWND hWnd = NULL;

	VkSurfaceKHR vulkanSurface = NULL;

	VkSwapchainKHR swapChain = NULL;
	VkImage* pVkSwapchainImageList = NULL;
	uint32_t numberOfSwapchainImages = 0;
	VkFormat swapChainImageFormat = 0;
	VkExtent2D swapChainExtend2D = { 0 };

	VkImageView* pSwapChainImageViewsList = NULL;

	VkRenderPass  renderPass = NULL;
	VkPipelineLayout pipelineLayout = NULL;
	VkPipeline pipeLine = NULL;

	VkFramebuffer* pFramebufferList = NULL;

	VkCommandPool commandPool = NULL;
	VkCommandBuffer commandBuffer = NULL;

	CreateVulkanInstance(&vulkanInstance);

	hWnd = CreateWindowInstance(L"Test Window", WindowProc);
	CreateVulkanSurface(hWnd, &vulkanSurface, &vulkanInstance);

	GetPhysicalDevice(&vulkanInstance, &physicalDevice);

	if (CheckDeviceExtensionsAvailability(&physicalDevice) == false)
	{
		error("Device Does not support the required Device Extensions.");
		return;
	}

	optional graphicsQueueFamilyIndex = { 0 };
	graphicsQueueFamilyIndex = GetPhysicalDeviceGraphicsQueueFamily(&physicalDevice);
	if (graphicsQueueFamilyIndex.hasValue == false)
	{
		error("GRAPHICS_BIT Queue Family not Found on Selected Device.");
		return;
	}

	optional presentationQueueFamilyIndex = { 0 };
	presentationQueueFamilyIndex = GetPhysicalDevicePresentationQueueFamily(&physicalDevice, &vulkanSurface);
	if (presentationQueueFamilyIndex.hasValue == false)
	{
		error("Selected Device Does not Support Presentation Queue Family.");
		return;
	}

	VkPhysicalDeviceFeatures physicalDeviceFeatures = { 0 };
	physicalDeviceFeatures = GetPhysicalDeviceFeatures(&physicalDevice);

	CreateLogicalDevice(&physicalDevice, &physicalDeviceFeatures, graphicsQueueFamilyIndex.value, presentationQueueFamilyIndex.value, &logicalDevice);

	GetDeviceQueue(&logicalDevice, graphicsQueueFamilyIndex.value, 0, &graphicsQueue);
	GetDeviceQueue(&logicalDevice, presentationQueueFamilyIndex.value, 0, &presentationQueue);

	CreateSwapchain(&physicalDevice, &logicalDevice, &vulkanSurface, &swapChain, &swapChainImageFormat, &swapChainExtend2D);
	pVkSwapchainImageList = GetSwapchainImages(&logicalDevice, &swapChain, &numberOfSwapchainImages);
	pSwapChainImageViewsList = CreateSwapchainImageViews(&logicalDevice, pVkSwapchainImageList, numberOfSwapchainImages, &swapChainImageFormat);

	CreateRenderPass(&logicalDevice, &swapChainImageFormat, &renderPass);
	CreatePipelineLayout(&logicalDevice, &pipelineLayout);
	CreatePipeline(&logicalDevice, &pipelineLayout, &renderPass, &swapChainExtend2D, &pipeLine);

	pFramebufferList = CreateFramebuffers(&logicalDevice, &renderPass, &swapChainExtend2D, pSwapChainImageViewsList, numberOfSwapchainImages);

	CreateCommandPool(&logicalDevice, &commandPool, graphicsQueueFamilyIndex.value);
	CreateCommandBuffer(&logicalDevice, &commandPool, &commandBuffer);

	RecordDrawCommand(&commandBuffer, &pipeLine, &renderPass, pFramebufferList, &swapChainExtend2D, 0);

	MSG	msg = {0};
	while (GetAndDispatchWindowMessage(hWnd, &msg) == true)
	{


		continue;
	}

	exitCode = (uint32_t) msg.wParam;

	DestroyCommandPool(&logicalDevice, &commandPool);
	DestroyAndFreeFramebuffers(&logicalDevice, pFramebufferList, numberOfSwapchainImages);
	DestroyPipeline(&logicalDevice, &pipeLine);
	DestroyRenderPass(&logicalDevice, &renderPass);
	DestroyPipelineLayout(&logicalDevice, &pipelineLayout);
	DestroyAndFreeSwapchainImageViews(&logicalDevice, pSwapChainImageViewsList, numberOfSwapchainImages);
	FreeSwapChainImages(pVkSwapchainImageList);
	DestroyVulkanSwapchain(&logicalDevice, &swapChain);
	DestroyVulkanDevice(&logicalDevice);
	DestroyVulkanSurface(&vulkanSurface, &vulkanInstance);
	DestroyWindow(hWnd);
	DestroyVulkanInstance(&vulkanInstance);

	return exitCode;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(CLOSE_UI_EXIT_CODE);
		return 0;
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}