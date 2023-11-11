#include "main.h"

int main()
{
	VkInstance vulkanInstance = NULL;
	VkPhysicalDevice physicalDevice = NULL;

	CreateVulkanInstance(&vulkanInstance);
	GetPhysicalDevice(&vulkanInstance, &physicalDevice);
	DestroyVulkanInstance(&vulkanInstance);

	return 0;
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
	
	ENABLED_EXTENSIONS;
	vkInstanceCreateInfo.ppEnabledExtensionNames = enabledExtensions;
	vkInstanceCreateInfo.enabledExtensionCount = ENABLED_EXTENSIONS_COUNT;
#ifdef DEBUG
	ENABLED_LAYERS;
	vkInstanceCreateInfo.ppEnabledLayerNames = enabledLayers;
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
	VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
	vkGetPhysicalDeviceProperties(*pVkPhysicalDevice, &vkPhysicalDeviceProperties);

	//VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	//vkGetPhysicalDeviceFeatures(*pVkPhysicalDevice, &vkPhysicalDeviceFeatures);

	printf("Device Name: %s\n", vkPhysicalDeviceProperties.deviceName);
}

void DestroyVulkanInstance(VkInstance* pVulkanInstance)
{
	log("Destroyed Vulkan Instance.");
	vkDestroyInstance(*pVulkanInstance, NULL);
}
