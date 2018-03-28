#pragma once

#include "device_local_buffer/create_device_local_buffer_with_data.h"

#include <utility>
#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace details
{
	struct SwapChainImagesResourcesData
	{
		std::vector<VkImage> images{};
		std::vector<VkImageView> imageViews{};
		std::vector<VkFramebuffer> framebuffers{};
		
		VkDevice device{VK_NULL_HANDLE};
		VkSwapchainKHR swapChain{VK_NULL_HANDLE};
		VkRenderPass renderPass{VK_NULL_HANDLE};
		VkSurfaceFormatKHR format{};
		uint32_t width{0};
		uint32_t height{0};
	};
	
	using MaybeSurfaceFormat = tl::expected<VkSurfaceFormatKHR, std::string>;
	using MaybePresentMode = tl::expected<VkPresentModeKHR, std::string>;
	using MaybeQueueFamilies = tl::expected<std::pair<uint32_t, uint32_t>, std::string>;
	using MaybeSwapChainResources = tl::expected<SwapChainImagesResourcesData, std::string>;
	
	MaybeSurfaceFormat get_device_surface_format(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	MaybePresentMode get_device_surface_present_mode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	MaybeQueueFamilies get_device_graphics_and_present_queue_families(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	bool check_required_device_extensions(VkPhysicalDevice physicalDevice, std::vector<char const *> const & requiredExtensions);
	bool check_device_suitability(VkPhysicalDevice physicalDevice, std::vector<char const *> const & requiredExtensions);
	VkExtent2D get_surface_extent(GLFWwindow * window, VkSurfaceCapabilitiesKHR const & surfaceCapabilities);
	MaybeSwapChainResources get_swap_chain_images(SwapChainImagesResourcesData resourcesData);
	MaybeSwapChainResources create_image_views(SwapChainImagesResourcesData resourcesData);
	MaybeSwapChainResources create_frame_buffers(SwapChainImagesResourcesData resourcesData);
	VkDeviceSize calculate_total_buffers_size(VkDevice device, std::vector<VkBuffer> const & buffers);
}