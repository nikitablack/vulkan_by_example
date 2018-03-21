#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace vk_helpers
{
	VkInstanceCreateInfo get_instance_create_info(std::vector<char const *> const * extensions = nullptr,
	                                              std::vector<char const *> const * layers = nullptr);
	VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t queueFamilyIndex, std::vector<float> const * queuePriorities);
	VkDeviceCreateInfo get_device_create_info(std::vector<VkDeviceQueueCreateInfo> const * queueCreateInfos,
	                                          std::vector<char const *> const * deviceExtensions,
	                                          VkPhysicalDeviceFeatures const * deviceFeatures);
	VkShaderModuleCreateInfo get_shader_module_create_info(std::vector<char> const * shaderCode);
}