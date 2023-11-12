#include "vulkan.h"

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
		return;
	}

	vkEnumerateDeviceExtensionProperties(*pVkPhysicalDevice, NULL, &numberOfExtensions, pVkExtensionsPropertiesList);

	bool result = TRUE;
	for (uint32_t x = 0; x < ENABLED_DEVICE_EXTENSIONS_COUNT; x++)
	{
		if (result == FALSE)
			break;

		for (uint32_t i = 0; i < numberOfExtensions; i++)
		{
			if (strcmp(pEnabledDeviceExtensions[x], &(pVkExtensionsPropertiesList[i].extensionName)) == 0)
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

	VkBool32 isSupported = 0;
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

void CreateVulkanInstance(VkInstance* pVulkanInstance)
{
	VkApplicationInfo vkApplicationInfo = { 0 };
	vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pApplicationName = "VulkanEngine";
	vkApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	vkApplicationInfo.pEngineName = "VulkanEngine";
	vkApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	vkApplicationInfo.apiVersion = VK_API_VERSION_1_0;

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

void DestroyVulkanSurface(VkSurfaceKHR* pVkSurface, VkInstance* pVkInstance)
{
	log("Vulkan Surface Destroyed.");
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
