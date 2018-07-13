#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace app::helpers
{

VkInstance create_instance(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
std::vector<VkPhysicalDevice> get_physical_devices(VkInstance instance);
std::vector<VkSurfaceFormatKHR> get_physical_devices_surface_formats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
std::vector<VkPresentModeKHR> get_physical_device_surface_present_modes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
VkDevice create_device(VkPhysicalDevice physicalDevice, std::vector<uint32_t> const * queueIndices, std::vector<std::vector<float>> const * queuePriorities, VkPhysicalDeviceFeatures const * features = nullptr, std::vector<char const *> const * extensions = nullptr);

} // namespace app::helpers