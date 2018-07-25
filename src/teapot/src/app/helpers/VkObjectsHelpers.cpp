#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"

#include <cassert>
#include <stdexcept>

using namespace std;
using namespace tl;

namespace app::helpers
{

MaybeInstance create_instance(vector<char const *> const * const extensions, vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
{
	VkInstanceCreateInfo const createInfo{get_instance_create_info(extensions, layers, applicationInfo)};
	
	VkInstance instance{VK_NULL_HANDLE};
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	return make_unexpected("failed to create instance");
	
	return instance;
}

MaybePhysicalDevices get_physical_devices(VkInstance const instance)
{
	assert(instance);
	
	uint32_t deviceCount{0};
	if(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
		return make_unexpected("failed to find GPUs with Vulkan support");
	
	vector<VkPhysicalDevice> physicalDevices(deviceCount);
	if(vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
		return make_unexpected("failed to find GPUs with Vulkan support");
	
	return physicalDevices;
}

MaybePhysicalDevicesSurfaceFormats get_physical_devices_surface_formats(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);
	
	uint32_t formatsCount{0};
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface formats");
	
	vector<VkSurfaceFormatKHR> formats(formatsCount);
	if (formatsCount == 0 || vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, formats.data()) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface formats");
	
	return formats;
}

MaybePhysicalDevicesSurfacePresentModes get_physical_device_surface_present_modes(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);
	
	uint32_t presentModesCount{0};
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface present modes");
	
	vector<VkPresentModeKHR> presentModes(presentModesCount);
	if (presentModesCount == 0 || vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data()) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface present modes");
	
	return presentModes;
}

vector<VkQueueFamilyProperties> get_queue_family_properties(VkPhysicalDevice const physicalDevice)
{
	uint32_t queueFamilyCount{0};
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	return queueFamilies;
}

MaybeDevice create_device(VkPhysicalDevice const physicalDevice, vector<uint32_t> const * const queueIndices, vector<vector<float>> const * const queuePriorities, VkPhysicalDeviceFeatures const * const features, vector<char const *> const * const extensions)
{
	assert(physicalDevice);
	assert(queueIndices);
	assert(!queueIndices->empty());
	assert(queuePriorities);
	assert(queueIndices->size() == queuePriorities->size());
	
	vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	queueCreateInfos.reserve(queueIndices->size());
	
	for(size_t i{0}; i < queueIndices->size(); ++i)
		queueCreateInfos.push_back(get_device_queue_create_info(queueIndices->at(i), &queuePriorities->at(i)));
	
	VkDeviceCreateInfo const createInfo{get_device_create_info(&queueCreateInfos, features, extensions)};
	
	VkDevice device{VK_NULL_HANDLE};
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		return make_unexpected("failed to create logical device");
	
	return device;
}

MaybeShaderModule create_shader_module(VkDevice const device, vector<char> const * const shaderCode)
{
	assert(device);
	
	VkShaderModuleCreateInfo createInfo{get_shader_module_create_info(shaderCode)};
	
	VkShaderModule shaderModule{VK_NULL_HANDLE};
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		return make_unexpected("failed to create shader module");
	
	return shaderModule;
}

MaybeDebugUtilsMessenger create_debug_utils_messenger(VkInstance const instance, VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity, VkDebugUtilsMessageTypeFlagsEXT const messageType, PFN_vkDebugUtilsMessengerCallbackEXT const pfnUserCallback, void * pUserData)
{
	PFN_vkCreateDebugUtilsMessengerEXT const func{reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"))};
	if (func == nullptr)
		return make_unexpected("failed to get debug utils messenger extension");
	
	VkDebugUtilsMessengerCreateInfoEXT const messengerInfo{get_debug_utils_messenger_create_info(messageSeverity, messageType, pfnUserCallback, pUserData)};
	
	VkDebugUtilsMessengerEXT messenger{VK_NULL_HANDLE};
	if(func(instance, &messengerInfo, nullptr, &messenger) != VK_SUCCESS)
		return make_unexpected("failed to create debug utils messenger");
	
	return messenger;
}

bool destroy_debug_utils_messenger(VkInstance const instance, VkDebugUtilsMessengerEXT const messenger)
{
	assert(instance);
	
	PFN_vkDestroyDebugUtilsMessengerEXT const func{reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))};
	
	assert(func);
	
	if (func == nullptr)
		return false;
	
	func(instance, messenger, nullptr);
	
	return true;
}

bool set_debug_utils_object_name(VkInstance const instance, VkDevice const device, VkObjectType const objectType, uint64_t const objectHandle, char const * const pObjectName)
{
	assert(instance);
	
	PFN_vkSetDebugUtilsObjectNameEXT const func{reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"))};
	
	assert(func);
	
	if (func == nullptr)
		return false;
	
	VkDebugUtilsObjectNameInfoEXT const info{get_debug_utils_object_name_info(objectType, objectHandle, pObjectName)};
	
	return func(device, &info) == VK_SUCCESS;
}

MaybeRenderPass create_render_pass(VkDevice const device, vector<VkSubpassDescription> const * const subpasses, vector<VkAttachmentDescription> const * const attachments, vector<VkSubpassDependency> const * const dependencies)
{
	assert(device);
	
	VkRenderPassCreateInfo const renderPassInfo{get_render_pass_create_info(subpasses, attachments, dependencies)};
	
	VkRenderPass renderPass{VK_NULL_HANDLE};
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		return make_unexpected("failed to create render pass");
	
	return renderPass;
}

MaybeDescriptorSetLayout create_descriptor_set_layout(VkDevice const device, vector<VkDescriptorSetLayoutBinding> const * const bindings)
{
	assert(device);
	
	VkDescriptorSetLayoutCreateInfo const layoutInfo{get_descriptor_set_layout_create_info(bindings)};
	
	VkDescriptorSetLayout descriptorSetLayout{VK_NULL_HANDLE};
	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
		return make_unexpected("failed to create descriptor set layout");
	
	return descriptorSetLayout;
}

MaybePipelineLayout create_pipeline_layout(VkDevice const device, vector<VkDescriptorSetLayout> const * const setLayouts, vector<VkPushConstantRange> const * const pushConstantRanges)
{
	assert(device);
	
	VkPipelineLayoutCreateInfo const pipelineLayoutInfo{get_pipeline_layout_create_info(setLayouts, pushConstantRanges)};
	
	VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		return make_unexpected("failed to create pipeline layout");
	
	return pipelineLayout;
}

MaybePipeline create_pipeline(VkDevice const device, VkRenderPass const renderPass, VkPipelineLayout const layout, uint32_t const subpass, VkPipelineCreateFlags const flags, vector<VkPipelineShaderStageCreateInfo> const * const shaderStages, VkPipelineVertexInputStateCreateInfo const * const  vertexInputState, VkPipelineInputAssemblyStateCreateInfo const * const inputAssemblyState, VkPipelineRasterizationStateCreateInfo const * const rasterizationState, VkPipelineColorBlendStateCreateInfo const * const colorBlendState, VkPipelineDepthStencilStateCreateInfo const * const depthStencilState, VkPipelineViewportStateCreateInfo const * const viewportState, VkPipelineMultisampleStateCreateInfo const * const multisampleState, VkPipelineTessellationStateCreateInfo const * const tesselationState, VkPipelineDynamicStateCreateInfo const * const dynamicState, VkPipeline const basePipelineHandle, VkPipelineCache const pipelineCache)
{
	assert(device);
	
	VkGraphicsPipelineCreateInfo info{get_graphics_pipeline_create_info(renderPass,
	                                                                    layout,
	                                                                    subpass,
	                                                                    flags,
	                                                                    shaderStages,
	                                                                    vertexInputState,
	                                                                    inputAssemblyState,
	                                                                    rasterizationState,
	                                                                    colorBlendState,
	                                                                    depthStencilState,
	                                                                    viewportState,
	                                                                    multisampleState,
	                                                                    tesselationState,
	                                                                    dynamicState,
	                                                                    basePipelineHandle,
	                                                                    -1)};
	
	VkPipeline pipeline{VK_NULL_HANDLE};
	if (vkCreateGraphicsPipelines(device, pipelineCache, 1, &info, nullptr, &pipeline) != VK_SUCCESS)
		return make_unexpected("failed to create graphics pipeline");
	
	return pipeline;
}

MaybeSwapchain create_swap_chain(VkDevice const device, VkSurfaceKHR const surface, uint32_t const imageCount, VkFormat const imageFormat, VkColorSpaceKHR const imageColorSpace, VkExtent2D const imageExtent, VkSurfaceTransformFlagBitsKHR const preTransform, VkPresentModeKHR const presentMode, VkSwapchainKHR const oldSwapchain, VkSharingMode const imageSharingMode, vector<uint32_t> const * const queueFamilyIndices, VkImageUsageFlags const imageUsage, uint32_t const imageArrayLayers, VkCompositeAlphaFlagBitsKHR const compositeAlpha, VkBool32 const clipped)
{
	assert(device);
	
	VkSwapchainCreateInfoKHR const createInfo{get_swap_chain_create_info(surface,
	                                                                     imageCount,
	                                                                     imageFormat,
	                                                                     imageColorSpace,
	                                                                     imageExtent,
	                                                                     preTransform,
	                                                                     presentMode,
	                                                                     imageSharingMode,
	                                                                     queueFamilyIndices,
	                                                                     imageUsage,
	                                                                     imageArrayLayers,
	                                                                     compositeAlpha,
	                                                                     clipped,
	                                                                     oldSwapchain)};
	
	VkSwapchainKHR swapChain{VK_NULL_HANDLE};
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		return tl::make_unexpected("failed to create swap chain");
	
	return swapChain;
}

bool destroy_swap_chain(VkInstance const instance, VkDevice const device, VkSwapchainKHR const swapChain)
{
	assert(instance);
	assert(device);
	
	PFN_vkDestroySwapchainKHR const func{reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetInstanceProcAddr(instance, "vkDestroySwapchainKHR"))};
	
	assert(func);
	
	if (func == nullptr)
		return false;
	
	func(device, swapChain, nullptr);
	
	return true;
}

MaybeImageView create_image_view(VkDevice const device, VkImage const image, VkImageViewType const viewType, VkFormat const format, VkImageSubresourceRange const subresourceRange, VkComponentMapping const components)
{
	assert(device);
	
	VkImageViewCreateInfo const createInfo{get_image_view_create_info(image, viewType, format, subresourceRange, components)};
	
	VkImageView imageView{VK_NULL_HANDLE};
	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS)
		return make_unexpected("failed to create image view");
	
	return imageView;
}

MaybeFrameBuffer create_framebuffer(VkDevice const device, VkRenderPass const renderPass, uint32_t const width, uint32_t const height, vector<VkImageView > const * const attachments, uint32_t const layers)
{
	assert(device);
	
	VkFramebufferCreateInfo const framebufferInfo{get_frame_buffer_create_info(renderPass, width, height, attachments, layers)};
	
	VkFramebuffer framebuffer{VK_NULL_HANDLE};
	if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS)
		return make_unexpected("failed to create framebuffer");
	
	return framebuffer;
}

MaybeBuffer create_buffer(VkDevice const device, VkDeviceSize const size, VkBufferUsageFlags const usage, VkSharingMode const sharingMode, vector<uint32_t> const * const queueFamilyIndices, VkBufferCreateFlags const flags, void const * const next)
{
	assert(device);
	
	VkBufferCreateInfo const bufferInfo{get_buffer_create_info(size, usage, sharingMode, queueFamilyIndices, flags, next)};
	
	VkBuffer buffer{VK_NULL_HANDLE};
	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		return make_unexpected("failed to create buffer");
	
	return buffer;
}

MaybeMemoryPropertyIndex get_supported_memory_property_index(VkPhysicalDevice const physicalDevice, uint32_t const supportedMemoryTypeBits, VkMemoryPropertyFlags const desiredMemoryFlags)
{
	VkPhysicalDeviceMemoryProperties memProperties{};
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	
	uint32_t memPropertyIndex{ 0 };
	for (uint32_t i{ 0 }; i < memProperties.memoryTypeCount; ++i)
	{
		if ((supportedMemoryTypeBits & (static_cast<uint32_t>(1) << i)) && (memProperties.memoryTypes[i].propertyFlags & desiredMemoryFlags) == desiredMemoryFlags)
		{
			memPropertyIndex = i;
			return memPropertyIndex;
		}
	}
	
	return make_unexpected("failed to find memory property index");
}

MaybeDeviceMemory allocate_device_memory(VkDevice const device, VkDeviceSize const allocationSize, uint32_t const memoryTypeIndex, void const * const next)
{
	assert(device);
	
	VkMemoryAllocateInfo const allocateInfo{get_memory_allocate_info(allocationSize, memoryTypeIndex, next)};
	
	VkDeviceMemory deviceMemory{ VK_NULL_HANDLE };
	if (vkAllocateMemory(device, &allocateInfo, nullptr, &deviceMemory) != VK_SUCCESS)
		return make_unexpected("failed to allocate buffer memory");
	
	return deviceMemory;
}

MaybeCommandPool create_command_pool(VkDevice const device, uint32_t const queueFamilyIndex, VkCommandPoolCreateFlags const flags)
{
	assert(device);
	
	VkCommandPoolCreateInfo const commandPoolCreateInfo{get_command_pool_create_info(queueFamilyIndex, flags)};
	
	VkCommandPool commandPool{ VK_NULL_HANDLE };
	if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
		return make_unexpected("failed to create command pool");
	
	return commandPool;
}

MaybeCommandBuffers allocate_command_buffers(VkDevice const device, VkCommandPool const commandPool, uint32_t const commandBufferCount, VkCommandBufferLevel const level)
{
	assert(device);
	assert(commandPool);
	
	VkCommandBufferAllocateInfo const commandBufferAllocateInfo{get_command_buffer_allocate_info(commandPool, commandBufferCount, level)};
	
	vector<VkCommandBuffer> commandBuffers(commandBufferCount);
	if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS)
		return make_unexpected("failed to allocate command buffer");
	
	return {commandBuffers};
}

MaybeDescriptorPool create_descriptor_pool(VkDevice const device, uint32_t const maxSets, std::vector<VkDescriptorPoolSize> const * const poolSizes)
{
	VkDescriptorPoolCreateInfo const poolInfo{get_descriptor_pool_create_info(maxSets, poolSizes)};
	
	VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
		return make_unexpected("failed to create descriptor pool");
	
	return descriptorPool;
}

MaybeDescriptorSets allocate_descriptor_sets(VkDevice const device, VkDescriptorPool const descriptorPool, vector<VkDescriptorSetLayout> const * const layouts)
{
	VkDescriptorSetAllocateInfo const allocateInfo{get_descriptor_set_allocate_info(descriptorPool, layouts)};
	
	vector<VkDescriptorSet> descriptorSets(allocateInfo.descriptorSetCount);
	if (vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) != VK_SUCCESS)
		return make_unexpected("failed to allocate descriptor set");
	
	return descriptorSets;
}

MaybeSemaphore create_semaphore(VkDevice const device)
{
	assert(device);
	
	VkSemaphoreCreateInfo const semaphoreInfo{get_semaphore_create_info()};
	
	VkSemaphore semaphore{VK_NULL_HANDLE};
	if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS)
		return make_unexpected("failed to create semaphore");
	
	return semaphore;
}

MaybeFence create_fence(VkDevice const device, VkFenceCreateFlags const flags)
{
	assert(device);
	
	VkFenceCreateInfo const fenceInfo{get_fence_create_info(flags)};
	
	VkFence fence{VK_NULL_HANDLE};
	if(vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS)
		return make_unexpected("failed to create fence");
	
	return fence;
}

} // namespace app::helpers