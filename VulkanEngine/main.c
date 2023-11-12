#include "main.h"

int main()
{
	int32_t exitCode = 0;

	VkInstance vulkanInstance = NULL;
	VkPhysicalDevice physicalDevice = NULL;
	VkDevice logicalDevice = NULL;
	VkQueue defaultQueue = NULL;

	HWND hWnd = NULL;

	VkSurfaceKHR vulkanSurface = NULL;

	CreateVulkanInstance(&vulkanInstance);

	hWnd = CreateWindowInstance(L"Test Window", WindowProc);
	CreateVulkanSurface(hWnd, &vulkanSurface, &vulkanInstance);

	GetPhysicalDevice(&vulkanInstance, &physicalDevice);

	VkPhysicalDeviceFeatures physicalDeviceFeatures = { 0 };
	optional queueFamilyIndex = { 0 };

	physicalDeviceFeatures = GetPhysicalDeviceFeatures(&physicalDevice);
	queueFamilyIndex = GetPhysicalDeviceQueueFamily(&physicalDevice);

	if (queueFamilyIndex.hasValue == FALSE)
	{
		error("GRAPHICS_BIT Queue Family not Found on Selected Device.");
		return;
	}

	CreateLogicalDevice(&physicalDevice, &physicalDeviceFeatures, queueFamilyIndex.value, &logicalDevice);
	GetDeviceQueue(&logicalDevice, queueFamilyIndex.value, 0, &defaultQueue);

	MSG	msg = {0};
	while (GetAndDispatchWindowMessage(hWnd, &msg) == TRUE);

	exitCode = msg.wParam;

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