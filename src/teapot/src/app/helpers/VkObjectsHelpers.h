#pragma once

#include "tl/expected.hpp"
#include "vulkan/vulkan.h"

#include <string>
#include <vector>

namespace app::helpers
{

using MaybeInstance = tl::expected<VkInstance, std::string>;
using MaybePhysicalDevices = tl::expected<std::vector<VkPhysicalDevice>, std::string>;
using MaybePhysicalDevicesSurfaceFormats = tl::expected<std::vector<VkSurfaceFormatKHR>, std::string>;
using MaybePhysicalDevicesSurfacePresentModes = tl::expected<std::vector<VkPresentModeKHR>, std::string>;
using MaybeDevice = tl::expected<VkDevice, std::string>;
using MaybeShaderModule = tl::expected<VkShaderModule, std::string>;

MaybeInstance create_instance(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
MaybePhysicalDevices get_physical_devices(VkInstance instance);
MaybePhysicalDevicesSurfaceFormats get_physical_devices_surface_formats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
MaybePhysicalDevicesSurfacePresentModes get_physical_device_surface_present_modes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
std::vector<VkQueueFamilyProperties> get_queue_family_properties(VkPhysicalDevice physicalDevice);
MaybeDevice create_device(VkPhysicalDevice physicalDevice, std::vector<uint32_t> const * queueIndices, std::vector<std::vector<float>> const * queuePriorities, VkPhysicalDeviceFeatures const * features = nullptr, std::vector<char const *> const * extensions = nullptr);
MaybeShaderModule create_shader_module(VkDevice device, std::vector<char> const * shaderCode);

} // namespace app::helpers