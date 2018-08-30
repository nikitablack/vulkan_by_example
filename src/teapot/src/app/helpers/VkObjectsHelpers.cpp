#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"

#include <cassert>
#include <stdexcept>

using namespace std;
using namespace tl;

namespace app::helpers
{

MaybeInstance create_instance(vector<char const *> const * const extensions, std::vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
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

MaybeExtensionProperties get_physical_device_device_extension_properties(VkPhysicalDevice const physicalDevice)
{
	assert(physicalDevice);
	
	uint32_t extensionCount{0};
	if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr) != VK_SUCCESS)
		return make_unexpected("failed to get physical device extension properties");
	
	vector<VkExtensionProperties> extensions(extensionCount);
	if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data()) != VK_SUCCESS)
		return make_unexpected("failed to get physical device extension properties");
	
	return extensions;
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

} // namespace app::helpers