#include "vulkan.h"

void DestroyRenderPass(VkDevice* pVkDevice, VkRenderPass* pVkRenderPass)
{
	log("Destroyed Render Pass.");
	vkDestroyRenderPass(*pVkDevice, *pVkRenderPass, NULL);
}

void CreateRenderPass(VkDevice* pVkDevice, VkFormat* pVkSwapchainImageFormat, VkRenderPass* pVkRenderPass)
{
	VkAttachmentDescription vkColorAttachmentDescription = { 0 };
	vkColorAttachmentDescription.format = *pVkSwapchainImageFormat;
	vkColorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	vkColorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vkColorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	vkColorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vkColorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkColorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkColorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference vkColorAttachmentReference = { 0 };
	vkColorAttachmentReference.attachment = 0;
	vkColorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription vkColorSubpassDescription = { 0 };
	vkColorSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	vkColorSubpassDescription.colorAttachmentCount = 1;
	vkColorSubpassDescription.pColorAttachments = &vkColorAttachmentReference;

	VkRenderPassCreateInfo vkRenderPassCreateInfo = { 0 };
	vkRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	vkRenderPassCreateInfo.attachmentCount = 1;
	vkRenderPassCreateInfo.pAttachments = &vkColorAttachmentDescription;
	vkRenderPassCreateInfo.subpassCount = 1;
	vkRenderPassCreateInfo.pSubpasses = &vkColorSubpassDescription;

	VkResult result = vkCreateRenderPass(*pVkDevice, &vkRenderPassCreateInfo, NULL, pVkRenderPass);

	if (result != VK_SUCCESS)
	{
		error("Failed to Create Render Pass.");
		return;
	}

	log("Render Pass Created.");
}

void CreatePipelineLayout(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout)
{
	VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo = { 0 };
	vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vkPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	vkPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
	vkPipelineLayoutCreateInfo.setLayoutCount = 0;
	vkPipelineLayoutCreateInfo.pSetLayouts = NULL;

	VkResult result = vkCreatePipelineLayout(*pVkDevice, &vkPipelineLayoutCreateInfo, NULL, pVkPipelineLayout);

	if (result != VK_SUCCESS)
	{
		error("Failed to Create Pipeline Layout.");
		return;
	}

	log("Pipeline Layout Created.");
}

void DestroyPipeline(VkDevice* pVkDevice, VkPipeline* pVkPipeline)
{
	log("Destroyed Pipeline.");
	vkDestroyPipeline(*pVkDevice, *pVkPipeline, NULL);
}

void CreatePipeline(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout, VkRenderPass* pVkRenderPass, VkExtent2D* pVkSwapchainExtent2D, VkPipeline* pVkPipeline)
{
	byte* pVertexShaderBytes = NULL;
	VkShaderModule vkVertexShaderModule = CreateShaderModule(pVkDevice, VERTEX_SHADER_PATH, &pVertexShaderBytes);

	byte* pFragmentShaderBytes = NULL;
	VkShaderModule vkFragmentShaderModule = CreateShaderModule(pVkDevice, FRAGMENT_SHADER_PATH, &pFragmentShaderBytes);

	VkPipelineShaderStageCreateInfo vkPipelineVertexShaderStageCreateInfo = { 0 };
	vkPipelineVertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineVertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vkPipelineVertexShaderStageCreateInfo.module = vkVertexShaderModule;
	vkPipelineVertexShaderStageCreateInfo.pName = VERTEX_SHADER_ENTRY_POINT;

	VkPipelineShaderStageCreateInfo vkPipelineFragmentShaderStageCreateInfo = { 0 };
	vkPipelineFragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vkPipelineFragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vkPipelineFragmentShaderStageCreateInfo.module = vkFragmentShaderModule;
	vkPipelineFragmentShaderStageCreateInfo.pName = FRAGMENT_SHADER_ENTRY_POINT;

	VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfoList[2] = {
		vkPipelineVertexShaderStageCreateInfo,
		vkPipelineFragmentShaderStageCreateInfo
	};

	VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo = { 0 };
	vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
	vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = NULL;
	vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
	vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = NULL;

	VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo = { 0 };
	vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo = { 0 };
	vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vkPipelineViewportStateCreateInfo.viewportCount = 1;
	vkPipelineViewportStateCreateInfo.scissorCount = 1;

#ifdef DYNAMIC_VIEW_PORT
	VkDynamicState vkDynamicStatesList[2] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateCreateInfo = { 0 };
	vkPipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	vkPipelineDynamicStateCreateInfo.dynamicStateCount = 2;
	vkPipelineDynamicStateCreateInfo.pDynamicStates = vkDynamicStatesList;
#endif

#ifndef DYNAMIC_VIEW_PORT
	VkViewport vkViewPort = { 0 };
	vkViewPort.width = (float)pVkSwapchainExtent2D->width;
	vkViewPort.height = (float)pVkSwapchainExtent2D->height;
	vkViewPort.x = 0.0f;
	vkViewPort.y = 0.0f;
	vkViewPort.maxDepth = 1.0f;
	vkViewPort.minDepth = 0.0f;

	VkRect2D vkScissor = { 0 };
	vkScissor.offset.x = 0;
	vkScissor.offset.y = 0;
	vkScissor.extent = *pVkSwapchainExtent2D;

	vkPipelineViewportStateCreateInfo.pViewports = &vkViewPort;
	vkPipelineViewportStateCreateInfo.pScissors = &vkScissor;
#endif

	VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo = { 0 };
	vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	vkPipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	vkPipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
	vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	vkPipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	vkPipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
	vkPipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	vkPipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo vkPipelineMultisamplesStateCreateInfo = { 0 };
	vkPipelineMultisamplesStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	vkPipelineMultisamplesStateCreateInfo.sampleShadingEnable = VK_FALSE;
	vkPipelineMultisamplesStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	vkPipelineMultisamplesStateCreateInfo.minSampleShading = 1.0f;
	vkPipelineMultisamplesStateCreateInfo.pNext = NULL;
	vkPipelineMultisamplesStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	vkPipelineMultisamplesStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState = { 0 };
	vkPipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
#ifndef ALPHA_BLENDING
	vkPipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
	vkPipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	vkPipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	vkPipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	vkPipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
#endif
#ifdef ALPHA_BLENDING
	vkPipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
	vkPipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	vkPipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
	vkPipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	vkPipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
#endif

	VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo = { 0 };
	vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
#ifdef COLOR_BLENDING
	vkPipelineColorBlendStateCreateInfo.logicOpEnable = VK_TRUE;
#endif
#ifndef COLOR_BLENDING
	vkPipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
#endif
	vkPipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	vkPipelineColorBlendStateCreateInfo.attachmentCount = 1;
	vkPipelineColorBlendStateCreateInfo.pAttachments = &vkPipelineColorBlendAttachmentState;
	vkPipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	vkPipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	vkPipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	vkPipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo = { 0 };
	vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vkGraphicsPipelineCreateInfo.stageCount = 2;
	vkGraphicsPipelineCreateInfo.pStages = vkPipelineShaderStageCreateInfoList;
	vkGraphicsPipelineCreateInfo.pVertexInputState = &vkPipelineVertexInputStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pViewportState = &vkPipelineViewportStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pMultisampleState = &vkPipelineMultisamplesStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pDepthStencilState = NULL;
	vkGraphicsPipelineCreateInfo.pColorBlendState = &vkPipelineColorBlendStateCreateInfo;
	vkGraphicsPipelineCreateInfo.pDynamicState = &vkPipelineDynamicStateCreateInfo;
	vkGraphicsPipelineCreateInfo.layout = *pVkPipelineLayout;
	vkGraphicsPipelineCreateInfo.renderPass = *pVkRenderPass;
	vkGraphicsPipelineCreateInfo.subpass = 0;
	vkGraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	vkGraphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkResult result = vkCreateGraphicsPipelines(*pVkDevice, VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, NULL, pVkPipeline);

	if (result != VK_SUCCESS)
	{
		error("Failed to Create Pipeline.");

		DestroyAndFreeShaderModule(pVkDevice, &vkVertexShaderModule, pVertexShaderBytes);
		DestroyAndFreeShaderModule(pVkDevice, &vkFragmentShaderModule, pFragmentShaderBytes);

		return;
	}

	DestroyAndFreeShaderModule(pVkDevice, &vkVertexShaderModule, pVertexShaderBytes);
	DestroyAndFreeShaderModule(pVkDevice, &vkFragmentShaderModule, pFragmentShaderBytes);

	log("Pipeline Created.");
}

void DestroyAndFreeShaderModule(VkDevice* pVkDevice, VkShaderModule* pVkShaderModule, byte* pShaderBytes)
{
	log("Destroyed Shader Module.");
	vkDestroyShaderModule(*pVkDevice, *pVkShaderModule, NULL);
	free(pShaderBytes);
}

VkShaderModule CreateShaderModule(VkDevice* pVkDevice, char* shaderName, byte** ppShaderBytes)
{
	uint32_t numberOfShaderBytes = 0;
	*ppShaderBytes = ReadBinary(shaderName, &numberOfShaderBytes);

	if (*ppShaderBytes == NULL)
	{
		error("Failed to Read Shader Binary.");
		return NULL;
	}

	VkShaderModule vkShaderModule = { 0 };

	VkShaderModuleCreateInfo vkShaderModuleCreateInfo = { 0 };
	vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vkShaderModuleCreateInfo.codeSize = numberOfShaderBytes;
	vkShaderModuleCreateInfo.pCode = (uint32_t*) *ppShaderBytes;
	
	VkResult result = vkCreateShaderModule(*pVkDevice, &vkShaderModuleCreateInfo, NULL, &vkShaderModule);
	if (result != VK_SUCCESS)
	{
		error("Failed To Create Shader Module.");
		return NULL;
	}

	logf("Shader Module Created: %s\n", shaderName);
	return vkShaderModule;
}

void DestroyAndFreeSwapchainImageViews(VkDevice* pVkDevice, VkImageView* pVkImageViewList, uint32_t numberOfImageViews)
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
		return false;
	}

	vkEnumerateDeviceExtensionProperties(*pVkPhysicalDevice, NULL, &numberOfExtensions, pVkExtensionsPropertiesList);

	bool result = true;
	for (uint32_t x = 0; x < ENABLED_DEVICE_EXTENSIONS_COUNT; x++)
	{
		if (result == false)
			break;

		const char* pCurrentEnabledDeviceExtension = pEnabledDeviceExtensions[x];

		for (uint32_t i = 0; i < numberOfExtensions; i++)
		{
			const char* pCurrentDeviceExtension = (char*) (&pVkExtensionsPropertiesList[i].extensionName);

			if (strcmp(pCurrentEnabledDeviceExtension, pCurrentDeviceExtension) == 0)
				break;
			else if (i == (numberOfExtensions - 1))
				result = false;
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
			result.hasValue = true;

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
	if (CheckDeviceExtensionsAvailability(pVkPhysicalDevice) == false)
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
	return true;
#endif

	ENABLED_EXTENSIONS;

	uint32_t numberOfExtensions = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &numberOfExtensions, NULL);

	VkExtensionProperties* pVkExtensionPropertiesList = calloc(numberOfExtensions, sizeof(VkExtensionProperties));
	if (pVkExtensionPropertiesList == NULL)
	{
		error("Failed to allocate memory for pVkExtensionPropertiesList (Instance Leve).");
		return false;
	}

	vkEnumerateInstanceExtensionProperties(NULL, &numberOfExtensions, pVkExtensionPropertiesList);
	bool result = true;
	for (uint32_t x = 0; x < ENABLED_EXTENSIONS_COUNT; x++)
	{
		if (result == false)
			break;

		const char* pCurrentEnabledDeviceExtension = pEnabledExtensions[x];

		for (uint32_t i = 0; i < numberOfExtensions; i++)
		{
			const char* pCurrentDeviceExtension = (char*) (&pVkExtensionPropertiesList[i].extensionName);

			if (strcmp(pCurrentEnabledDeviceExtension, pCurrentDeviceExtension) == 0)
				break;
			else if (i == (numberOfExtensions - 1))
				result = false;
		}
	}

	return result;
}

void CreateVulkanInstance(VkInstance* pVulkanInstance)
{
	if (CheckInstanceExtensionsAvailability() == false)
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
			result.hasValue = true;
		}
#pragma warning(pop)
	}

	free(pQueueFamiliesList);

	return result;
}

void DestroyPipelineLayout(VkDevice* pVkDevice, VkPipelineLayout* pVkPipelineLayout)
{
	log("Destroyed Pipeline Layout.");

	vkDestroyPipelineLayout(*pVkDevice, *pVkPipelineLayout, NULL);
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