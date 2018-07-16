#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace app::helpers
{

VkInstanceCreateInfo get_instance_create_info(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t queueFamilyIndex, std::vector<float> const * queuePriorities);
VkDeviceCreateInfo get_device_create_info(std::vector<VkDeviceQueueCreateInfo> const * queueCreateInfos, VkPhysicalDeviceFeatures const * deviceFeatures = nullptr, std::vector<char const *> const * deviceExtensions = nullptr);
VkShaderModuleCreateInfo get_shader_module_create_info(std::vector<char> const * shaderCode);

} // namespace app::helpers