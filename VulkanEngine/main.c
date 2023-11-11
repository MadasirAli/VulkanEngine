#include "main.h"

int main()
{
	VkInstance vulkanInstance = NULL;
	VkPhysicalDevice physicalDevice = NULL;
	VkDevice logicalDevice = NULL;

	CreateVulkanInstance(&vulkanInstance);

	GetPhysicalDevice(&vulkanInstance, &physicalDevice);

	VkPhysicalDeviceFeatures physicalDeviceFeatures = { 0 };
	optional queueFamilyIndex = { 0 };

	physicalDeviceFeatures = GetPhysicalDeviceFeatures(&physicalDevice);
	queueFamilyIndex = GetPhysicalDeviceQueueFamily(&physicalDevice);

	CreateLogicalDevice(&physicalDevice, &physicalDeviceFeatures, &queueFamilyIndex, &logicalDevice);

	DestroyVulkanDevice(&logicalDevice);
	DestroyVulkanInstance(&vulkanInstance);

	return 0;
}

void CreateLogicalDevice(VkPhysicalDevice* pVkPhysicalDevice, VkPhysicalDeviceFeatures* pVkPhysicalDeviceFeatures, optional* pVkQueueFamilyIndex, VkDevice* pVkDevice)
{
	VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo = { 0 };
	vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

	optional queueFamilyIndex = *pVkQueueFamilyIndex;

	if (queueFamilyIndex.hasValue == false)
	{
		error("GRAPHICS_BIT Queue Family not Found on Selected Device.");
		return;
	}

	vkDeviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex.value;
	vkDeviceQueueCreateInfo.queueCount = 1;

	const float QueuePriority = QUEUE_PRIORITY;
	vkDeviceQueueCreateInfo.pQueuePriorities = &QueuePriority;

	VkDeviceCreateInfo vkDeviceCreateInfo = { 0 };
	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	vkDeviceCreateInfo.pQueueCreateInfos = &vkDeviceQueueCreateInfo;
	vkDeviceCreateInfo.queueCreateInfoCount = 1;

	vkDeviceCreateInfo.pEnabledFeatures = pVkPhysicalDeviceFeatures;

#ifdef ENABLE_EXTENSIONS
	ENABLED_EXTENSIONS;
	vkDeviceCreateInfo.ppEnabledExtensionNames = pEnabledExtensions;
	vkDeviceCreateInfo.enabledExtensionCount = ENABLED_EXTENSIONS_COUNT;
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
		return;
	}

	log("Logical Device Created.");
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

	VkPhysicalDevice* pPhysicalDeviceList = malloc(sizeof(VkPhysicalDevice) * numberOfDevices);

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

optional GetPhysicalDeviceQueueFamily(VkPhysicalDevice* pVkPhysicalDevice)
{
	optional result = { 0 };

	uint32_t numberOfQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*pVkPhysicalDevice, &numberOfQueueFamilies, NULL);

	VkQueueFamilyProperties* pQueueFamiliesList = malloc(sizeof(VkQueueFamilyProperties) * numberOfQueueFamilies);

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
			result.hasValue = true;
		}
#pragma warning(pop)
	}

	free(pQueueFamiliesList);

	return result;
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
