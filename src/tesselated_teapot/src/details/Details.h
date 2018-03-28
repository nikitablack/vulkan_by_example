#pragma once

#include "tl/expected.hpp"

#include <string>
#include <utility>
#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace details
{
	using MaybeSurfaceFormat = tl::expected<VkSurfaceFormatKHR, std::string>;
	using MaybePresentMode = tl::expected<VkPresentModeKHR, std::string>;
	using MaybeQueueFamilies = tl::expected<std::pair<uint32_t, uint32_t>, std::string>;
	
	MaybeSurfaceFormat get_device_surface_format(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	MaybePresentMode get_device_surface_present_mode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	MaybeQueueFamilies get_device_graphics_and_present_queue_families(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	bool check_required_device_extensions(VkPhysicalDevice physicalDevice, std::vector<char const *> const & requiredExtensions);
	bool check_device_suitability(VkPhysicalDevice physicalDevice, std::vector<char const *> const & requiredExtensions);
	VkExtent2D get_surface_extent(GLFWwindow * window, VkSurfaceCapabilitiesKHR const & surfaceCapabilities);
}