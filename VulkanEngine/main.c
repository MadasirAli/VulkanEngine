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

	CreateVulkanInstance(&vulkanInstance);

	hWnd = CreateWindowInstance(L"Test Window", WindowProc);
	CreateVulkanSurface(hWnd, &vulkanSurface, &vulkanInstance);

	GetPhysicalDevice(&vulkanInstance, &physicalDevice);

	if (CheckDeviceExtensionsAvailability(&physicalDevice) == FALSE)
	{
		error("Device Does not support the required Device Extensions.");
		return;
	}

	optional graphicsQueueFamilyIndex = { 0 };
	graphicsQueueFamilyIndex = GetPhysicalDeviceGraphicsQueueFamily(&physicalDevice);
	if (graphicsQueueFamilyIndex.hasValue == FALSE)
	{
		error("GRAPHICS_BIT Queue Family not Found on Selected Device.");
		return;
	}

	optional presentationQueueFamilyIndex = { 0 };
	presentationQueueFamilyIndex = GetPhysicalDevicePresentationQueueFamily(&physicalDevice, &vulkanSurface);
	if (presentationQueueFamilyIndex.hasValue == FALSE)
	{
		error("Selected Device Does not Support Presentation Queue Family.");
		return;
	}

	VkPhysicalDeviceFeatures physicalDeviceFeatures = { 0 };
	physicalDeviceFeatures = GetPhysicalDeviceFeatures(&physicalDevice);

	CreateLogicalDevice(&physicalDevice, &physicalDeviceFeatures, graphicsQueueFamilyIndex.value, presentationQueueFamilyIndex.value, &logicalDevice);

	GetDeviceQueue(&logicalDevice, graphicsQueueFamilyIndex.value, 0, &graphicsQueue);
	GetDeviceQueue(&logicalDevice, presentationQueueFamilyIndex.value, 0, &presentationQueue);

	MSG	msg = {0};
	while (GetAndDispatchWindowMessage(hWnd, &msg) == TRUE);

	exitCode = (uint32_t) msg.wParam;

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
		PostQuitMessage(1);
		return 0;
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}