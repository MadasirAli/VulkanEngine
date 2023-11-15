#include "vulkan.h"

void FreeSwapchainImageViews(VkDevice* pVkDevice, VkImageView* pVkImageViewList, uint32_t numberOfImageViews)
{
	log("Destroyed Image Views.");
	for (uint32_t i = 0; i < numberOfImageViews; i++)
	{
		vkDestroyImageView(*pVkDevice, pVkImageViewList[i], NULL);
	}

	free(pVkImageViewList);
}

VkImageView* CreateSwapchainImageViews(VkDevice* pVkDevice, VkImage* pVkSwapchainImages, uint32_t numberOfImages, VkFormat* pVkImageFormat)
{
	VkResult result = { 0 };

	VkImageView* pVkImageViewList = calloc(numberOfImages, sizeof(VkImageView));

	if (pVkImageViewList == NULL)
	{
		error("Failed to allocate memory pVkImageViewList.");
		return NULL;
	}

	for (uint32_t i = 0; i < numberOfImages; i++)
	{
		VkImageViewCreateInfo vkImageViewCreateInfo = { 0 };
		vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkImageViewCreateInfo.image = pVkSwapchainImages[i];
		vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		vkImageViewCreateInfo.format = *pVkImageFormat;
		vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		vkImageViewCreateInfo.subresourceRange.layerCount = 1;
		vkImageViewCreateInfo.subresourceRange.levelCount = 1;
		
		result = vkCreateImageView(*pVkDevice, &vkImageViewCreateInfo, NULL, &pVkImageViewList[i]);

		if (result != VK_SUCCESS)
		{
			error("Failed to Create Image View.");
			return NULL;
		}
	}

	log("Created Image Views.");

	return pVkImageViewList;
}

void FreeSwapChainImages(VkImage* pVkImageList)
{
	if (pVkImageList == NULL)
	{
		error("pVkImageList is a Null Pointer, Failed to Free allocated memory.");
		return;
	}

	free(pVkImageList);
}

VkImage* GetSwapchainImages(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchainKHR, uint32_t* numberOfImages)
{
	VkResult result = { 0 };

	result = vkGetSwapchainImagesKHR(*pVkDevice, *pVkSwapchainKHR, numberOfImages, NULL);

	if (result != VK_SUCCESS)
	{
		error("Failed to get number of images in swap chain");
		return NULL;
	}

	VkImage* pVkImageList = calloc(*numberOfImages, sizeof(VkImage));

	if (pVkImageList == NULL)
	{
		error("Failed to allocate memory for pVkImageList.");
		return NULL;
	}

	result = vkGetSwapchainImagesKHR(*pVkDevice, *pVkSwapchainKHR, numberOfImages, pVkImageList);

	if (result != VK_SUCCESS)
	{
		error("Failed to get swap chain images in list (allocated memory).");
		return NULL;
	}

	return pVkImageList;
}

void CreateSwapchain(VkPhysicalDevice* pVkPhysicalDevice, VkDevice* pVkDevice, VkSurfaceKHR* pVkSurfaceKHR, VkSwapchainKHR* pVkSwapchainKHR, VkFormat* pVkFormat, VkExtent2D* pVkExtend2D)
{
	VkResult result = {0};

	VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR = { 0 };
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*pVkPhysicalDevice, *pVkSurfaceKHR, &vkSurfaceCapabilitiesKHR);

	if (result != VK_SUCCESS)
	{
		error("Enable to get physical device surface capabilities");
		return;
	}

	if (vkSurfaceCapabilitiesKHR.currentExtent.width == MAXUINT32)
	{
		error("Needs Surface Capabilities Extend2D to be redefined.");
		return;
	}

	VkExtent2D windowSize = { 0 };
	windowSize = vkSurfaceCapabilitiesKHR.currentExtent;
	uint32_t imageCount = 0;
	if (vkSurfaceCapabilitiesKHR.maxImageCount > 0 && vkSurfaceCapabilitiesKHR.minImageCount + 1 <= vkSurfaceCapabilitiesKHR.maxImageCount)
		imageCount = vkSurfaceCapabilitiesKHR.minImageCount + 1;
	else imageCount = vkSurfaceCapabilitiesKHR.maxImageCount;

	uint32_t numberofSurfaceFormats = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(*pVkPhysicalDevice, *pVkSurfaceKHR, &numberofSurfaceFormats, NULL);

	if (result != VK_SUCCESS)
	{
		error("Failed to GetPhysicalDeviceSurfaceFormatesKHR Count");
		return;
	}

	VkSurfaceFormatKHR* pVkSurfaceFormatKHRList = calloc(numberofSurfaceFormats, sizeof(VkSurfaceFormatKHR));
	if (pVkSurfaceFormatKHRList == NULL)
	{
		error("Failed to allocate memory for pVkSurfaceFormatKHRList.");
		return;
	}

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(*pVkPhysicalDevice, *pVkSurfaceKHR, &numberofSurfaceFormats, pVkSurfaceFormatKHRList);

	if (result != VK_SUCCESS)
	{
		error("Failed to GetPhysicalDeviceSurfaceFormatsKHR in List (allocated memory).");
		return;
	}

	VkSurfaceFormatKHR surfaceFormat = { 0 };
	for (uint32_t i = 0; i < numberofSurfaceFormats; i++)
	{
		if (pVkSurfaceFormatKHRList[i].format == VK_FORMAT_R8G8B8A8_SRGB && pVkSurfaceFormatKHRList[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			surfaceFormat = pVkSurfaceFormatKHRList[i];
			break;
		}
		else if (i == (numberofSurfaceFormats - 1))
			surfaceFormat = pVkSurfaceFormatKHRList[0];
	}

	uint32_t numberOfPresentModes = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(*pVkPhysicalDevice, *pVkSurfaceKHR, &numberOfPresentModes, NULL);

	if (result != VK_SUCCESS)
	{
		error("Failed to GetPhysicalDeviceSurfacePresentModesKHR count.");
		return;
	}

	VkPresentModeKHR* pVkPresentModeKHRList = calloc(numberOfPresentModes, sizeof(VkPresentModeKHR));
	if (pVkPresentModeKHRList == NULL)
	{
		error("Failed to allocate memory for pVkPresentModeKHRList.");
		return;
	}

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(*pVkPhysicalDevice, *pVkSurfaceKHR, &numberOfPresentModes, pVkPresentModeKHRList);

	if (result != VK_SUCCESS)
	{
		error("Failed to GetPhysicalDeviceSurfacePresentModesKHR in list (allocated memory).");
		return;
	}

	VkPresentModeKHR presentMode = { 0 };
	for (uint32_t i = 0; i < numberOfPresentModes; i++)
	{
		if (pVkPresentModeKHRList[i] == VK_PRESENT_MODE_FIFO_KHR)
		{
			presentMode = pVkPresentModeKHRList[i];
			break;
		}
	}

	uint32_t graphicsQueueFamilyIndex = GetPhysicalDeviceGraphicsQueueFamily(pVkPhysicalDevice).value;
	uint32_t presentationQueueFamilyIndex = GetPhysicalDevicePresentationQueueFamily(pVkPhysicalDevice, pVkSurfaceKHR).value;

	VkSwapchainCreateInfoKHR vkSwapChainCreateInfoKHR = { 0 };

	vkSwapChainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkSwapChainCreateInfoKHR.surface = *pVkSurfaceKHR;
	vkSwapChainCreateInfoKHR.minImageCount = imageCount;
	vkSwapChainCreateInfoKHR.imageFormat = surfaceFormat.format;
	vkSwapChainCreateInfoKHR.imageColorSpace = surfaceFormat.colorSpace;
	vkSwapChainCreateInfoKHR.imageExtent = windowSize;
	vkSwapChainCreateInfoKHR.imageArrayLayers = 1;
	vkSwapChainCreateInfoKHR.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (graphicsQueueFamilyIndex != presentationQueueFamilyIndex)
	{
		vkSwapChainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		vkSwapChainCreateInfoKHR.queueFamilyIndexCount = 2;

		const uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentationQueueFamilyIndex };
		vkSwapChainCreateInfoKHR.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		vkSwapChainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkSwapChainCreateInfoKHR.queueFamilyIndexCount = 0;
		vkSwapChainCreateInfoKHR.pQueueFamilyIndices = NULL;
	}

	vkSwapChainCreateInfoKHR.preTransform = vkSurfaceCapabilitiesKHR.currentTransform;
	vkSwapChainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	vkSwapChainCreateInfoKHR.presentMode = presentMode;
	vkSwapChainCreateInfoKHR.clipped = VK_TRUE;

	vkSwapChainCreateInfoKHR.oldSwapchain = VK_NULL_HANDLE;

	result = vkCreateSwapchainKHR(*pVkDevice, &vkSwapChainCreateInfoKHR, NULL, pVkSwapchainKHR);

	if (result != VK_SUCCESS)
	{
		error("Failed to Create Swap Chain.");
		return;
	}

	log("Swap Chain Created.");

	*pVkExtend2D = windowSize;
	*pVkFormat = surfaceFormat.format;
}

bool CheckDeviceExtensionsAvailability(VkPhysicalDevice* pVkPhysicalDevice)
{
#ifdef ENABLE_DEVICE_EXTENSIONS
	ENABLED_DEVICE_EXTENSIONS;
	uint32_t numberOfExtensions = 0;
	vkEnumerateDeviceExtensionProperties(*pVkPhysicalDevice, NULL, &numberOfExtensions, NULL);

	VkExtensionProperties* pVkExtensionsPropertiesList = calloc(numberOfExtensions, sizeof(VkExtensionProperties));
	if (pVkExtensionsPropertiesList == NULL)
	{
		error("Failed to allocate memory for pVkExtensionsPropertiesList.");
		return FALSE;
	}

	vkEnumerateDeviceExtensionProperties(*pVkPhysicalDevice, NULL, &numberOfExtensions, pVkExtensionsPropertiesList);

	bool result = TRUE;
	for (uint32_t x = 0; x < ENABLED_DEVICE_EXTENSIONS_COUNT; x++)
	{
		if (result == FALSE)
			break;

		const char* pCurrentEnabledDeviceExtension = pEnabledDeviceExtensions[x];

		for (uint32_t i = 0; i < numberOfExtensions; i++)
		{
			const char* pCurrentDeviceExtension = (char*) (&pVkExtensionsPropertiesList[i].extensionName);

			if (strcmp(pCurrentEnabledDeviceExtension, pCurrentDeviceExtension) == 0)
				break;
			else if (i == (numberOfExtensions - 1))
				result = FALSE;
		}
	}

	free(pVkExtensionsPropertiesList);
	return result;
#endif
}

optional GetPhysicalDevicePresentationQueueFamily(VkPhysicalDevice* pVkPhysicalDevice, VkSurfaceKHR* pVkSurface)
{
	optional result = { 0 };

	uint32_t numberOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*pVkPhysicalDevice, &numberOfQueueFamilies, NULL);

	VkBool32 isSupported = VK_FALSE;
	for (uint32_t i = 0; i < numberOfQueueFamilies; i++)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(*pVkPhysicalDevice, i, *pVkSurface, &isSupported);
		if (isSupported)
		{
			result.value = i;
			result.hasValue = TRUE;

			break;
		}
	}

	return result;
}

void CreateVulkanSurface(HWND hWnd, VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance)
{
	VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfo = { 0 };
	vkWin32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vkWin32SurfaceCreateInfo.hinstance = GetModuleHandleA(NULL);
	vkWin32SurfaceCreateInfo.hwnd = hWnd;

	VkResult result = vkCreateWin32SurfaceKHR(*pVkInstance, &vkWin32SurfaceCreateInfo, NULL, pVkSurface);

	if (result == VK_SUCCESS)
		log("Vulkan Win32 Surface Created.");
	else
		error("Failed to Create Win32 Vulkan Surface.");;

	return;
}

void GetDeviceQueue(VkDevice* pVkDevice, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pVkQueue)
{
	if (*pVkDevice == NULL)
	{
		error("Null Pointer of Logical Device while getting a queue.");
		return;
	}

	vkGetDeviceQueue(*pVkDevice, queueFamilyIndex, queueIndex, pVkQueue);
}

void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, uint32_t graphicsQueueFamilyIndex, uint32_t presentationQueueFamilyIndex,VkDevice* pVkDevice)
{
	if (CheckDeviceExtensionsAvailability(pVkPhysicalDevice) == FALSE)
	{
		error("Selected Physical Device Does not support the required device level Extensions.");
		return;
	}

	uint32_t queueFamiliesCount = 2;

	if (graphicsQueueFamilyIndex == presentationQueueFamilyIndex)
		queueFamiliesCount--;

	VkDeviceQueueCreateInfo* pVkDeviceQueueCreateInfoList = calloc(queueFamiliesCount, sizeof(VkDeviceQueueCreateInfo));

	if (pVkDeviceQueueCreateInfoList == NULL)
	{
		error("Failed to allocate memory for vkDeviceQueueCreateInfoList");
		return;
	}

	pVkDeviceQueueCreateInfoList[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	pVkDeviceQueueCreateInfoList[0].queueFamilyIndex = graphicsQueueFamilyIndex;
	pVkDeviceQueueCreateInfoList[0].queueCount = 1;
	const float graphicsQueuePriority = GRAPHICS_QUEUE_PRIORITY;
	pVkDeviceQueueCreateInfoList[0].pQueuePriorities = &graphicsQueuePriority;

	if (queueFamiliesCount > 1)
	{
		pVkDeviceQueueCreateInfoList[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		pVkDeviceQueueCreateInfoList[1].queueFamilyIndex = presentationQueueFamilyIndex;
		pVkDeviceQueueCreateInfoList[1].queueCount = 1;
		const float presentationQueuePriority = PRESENTATION_QUEUE_PRIORITY;
		pVkDeviceQueueCreateInfoList[1].pQueuePriorities = &presentationQueuePriority;
	}

	VkDeviceCreateInfo vkDeviceCreateInfo = { 0 };
	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	vkDeviceCreateInfo.pQueueCreateInfos = pVkDeviceQueueCreateInfoList;
	vkDeviceCreateInfo.queueCreateInfoCount = queueFamiliesCount;

	vkDeviceCreateInfo.pEnabledFeatures = pVkPhysicalDeviceFeatures;

#ifdef ENABLE_DEVICE_EXTENSIONS
	ENABLED_DEVICE_EXTENSIONS;
	vkDeviceCreateInfo.ppEnabledExtensionNames = pEnabledDeviceExtensions;
	vkDeviceCreateInfo.enabledExtensionCount = ENABLED_DEVICE_EXTENSIONS_COUNT;
#endif

#ifdef _DEBUG
	ENABLED_LAYERS;
	vkDeviceCreateInfo.ppEnabledLayerNames = pEnabledLayers;
	vkDeviceCreateInfo.enabledLayerCount = ENABLED_LAYERS_COUNT;
#endif

	VkResult result = vkCreateDevice(*pVkPhysicalDevice, &vkDeviceCreateInfo, NULL, pVkDevice);

	if (result != VK_SUCCESS)
	{
		error("Failed to Create Logical Device.");
		free(pVkDeviceQueueCreateInfoList);
		return;
	}

	log("Logical Device Created.");
	free(pVkDeviceQueueCreateInfoList);
}

bool CheckInstanceExtensionsAvailability()
{
#ifndef ENABLED_EXTENSIONS
	return TRUE;
#endif

	ENABLED_EXTENSIONS;

	uint32_t numberOfExtensions = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &numberOfExtensions, NULL);

	VkExtensionProperties* pVkExtensionPropertiesList = calloc(numberOfExtensions, sizeof(VkExtensionProperties));
	if (pVkExtensionPropertiesList == NULL)
	{
		error("Failed to allocate memory for pVkExtensionPropertiesList (Instance Leve).");
		return FALSE;
	}

	vkEnumerateInstanceExtensionProperties(NULL, &numberOfExtensions, pVkExtensionPropertiesList);
	bool result = TRUE;
	for (uint32_t x = 0; x < ENABLED_EXTENSIONS_COUNT; x++)
	{
		if (result == FALSE)
			break;

		const char* pCurrentEnabledDeviceExtension = pEnabledExtensions[x];

		for (uint32_t i = 0; i < numberOfExtensions; i++)
		{
			const char* pCurrentDeviceExtension = (char*) (&pVkExtensionPropertiesList[i].extensionName);

			if (strcmp(pCurrentEnabledDeviceExtension, pCurrentDeviceExtension) == 0)
				break;
			else if (i == (numberOfExtensions - 1))
				result = FALSE;
		}
	}

	return result;
}

void CreateVulkanInstance(VkInstance* pVulkanInstance)
{
	if (CheckInstanceExtensionsAvailability() == FALSE)
	{
		error("System Does not Support the Required Instance Extensions");
		return;
	}

	VkApplicationInfo vkApplicationInfo = { 0 };
	vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pApplicationName = "VulkanEngine";
	vkApplicationInfo.applicationVersion = API_VERSION;
	vkApplicationInfo.pEngineName = "VulkanEngine";
	vkApplicationInfo.engineVersion = API_VERSION;
	vkApplicationInfo.apiVersion = API_VERSION;

	VkInstanceCreateInfo vkInstanceCreateInfo = { 0 };
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;

#ifdef ENABLE_EXTENSIONS
	ENABLED_EXTENSIONS;
	vkInstanceCreateInfo.ppEnabledExtensionNames = pEnabledExtensions;
	vkInstanceCreateInfo.enabledExtensionCount = ENABLED_EXTENSIONS_COUNT;
#endif
#ifdef _DEBUG
	ENABLED_LAYERS;
	vkInstanceCreateInfo.ppEnabledLayerNames = pEnabledLayers;
	vkInstanceCreateInfo.enabledLayerCount = ENABLED_LAYERS_COUNT;
#endif

	VkResult result = vkCreateInstance(&vkInstanceCreateInfo, NULL, pVulkanInstance);

	if (result != VK_SUCCESS)
		error("Failed to Create Vulkan Instance.");
	else
		log("Created Vulkan Instance.");
}
void GetPhysicalDevice(VkInstance* pVulkanInstance, VkPhysicalDevice* pVkPhysicalDevice)
{
	VkResult result = { 0 };

	uint32_t numberOfDevices = 0;
	result = vkEnumeratePhysicalDevices(*pVulkanInstance, &numberOfDevices, NULL);

	if (result != VK_SUCCESS)
	{
		error("Failed to get Physical Devices Count.");
		return;
	}

	if (numberOfDevices == 0)
	{
		error("No Vulkan Supported Device Found.");
		return;
	}

	VkPhysicalDevice* pPhysicalDeviceList = calloc(numberOfDevices, sizeof(VkPhysicalDevice));

	if (pPhysicalDeviceList == NULL)
	{
		error("Unable to allocate memory for Physical Device List.");
		return;
	}

	result = vkEnumeratePhysicalDevices(*pVulkanInstance, &numberOfDevices, pPhysicalDeviceList);

	if (result != VK_SUCCESS)
	{
		error("Failed to Enumerate Physical Devices.");
		return;
	}

	*pVkPhysicalDevice = *pPhysicalDeviceList;

	log("Selected Physical Device 0");

	ShowPhysicalDeviceDetails(pVkPhysicalDevice);

	free(pPhysicalDeviceList);
}

void ShowPhysicalDeviceDetails(VkPhysicalDevice* pVkPhysicalDevice)
{
	VkPhysicalDeviceProperties vkPhysicalDeviceProperties = { 0 };
	vkGetPhysicalDeviceProperties(*pVkPhysicalDevice, &vkPhysicalDeviceProperties);

	log("Using Device:");
	log(vkPhysicalDeviceProperties.deviceName);
}

VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice* pVkPhysicalDevice)
{
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = { 0 };
	vkGetPhysicalDeviceFeatures(*pVkPhysicalDevice, &vkPhysicalDeviceFeatures);

	return vkPhysicalDeviceFeatures;
}

optional GetPhysicalDeviceGraphicsQueueFamily(VkPhysicalDevice* pVkPhysicalDevice)
{
	optional result = { 0 };

	uint32_t numberOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*pVkPhysicalDevice, &numberOfQueueFamilies, NULL);

	VkQueueFamilyProperties* pQueueFamiliesList = calloc(numberOfQueueFamilies, sizeof(VkQueueFamilyProperties));

	if (pQueueFamiliesList == NULL)
	{
		error("Failed to allocate memory for QueueFamilyProperties List.");
		return result;
	}

	vkGetPhysicalDeviceQueueFamilyProperties(*pVkPhysicalDevice, &numberOfQueueFamilies, pQueueFamiliesList);

	for (uint32_t i = 0; i < numberOfQueueFamilies; i++)
	{
#pragma warning(push)
#pragma warning(disable:6385)
		if (pQueueFamiliesList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			result.value = i;
			result.hasValue = TRUE;
		}
#pragma warning(pop)
	}

	free(pQueueFamiliesList);

	return result;
}

void DestroyVulkanSwapchain(VkDevice* pVkDevice, VkSwapchainKHR* pVkSwapchain)
{
	log("Destroyed Vulkan Swapchain.");
	vkDestroySwapchainKHR(*pVkDevice, *pVkSwapchain, NULL);
}

void DestroyVulkanSurface(VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance)
{
	log("Destroyed Vulkan Surface.");
	vkDestroySurfaceKHR(*pVkInstance, *pVkSurface, NULL);
}

void DestroyVulkanDevice(VkDevice* pVkDevice)
{
	log("Destroyed Logical Device.");
	vkDestroyDevice(*pVkDevice, NULL);
}

void DestroyVulkanInstance(VkInstance* pVulkanInstance)
{
	log("Destroyed Vulkan Instance.");
	vkDestroyInstance(*pVulkanInstance, NULL);
}
