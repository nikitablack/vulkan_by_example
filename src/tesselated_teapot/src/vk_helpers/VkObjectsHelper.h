#pragma once

#include "tl/expected.hpp"

#include <string>
#include <vulkan/vulkan.hpp>

namespace vk_helpers
{
	using MaybeShaderModule = tl::expected<VkShaderModule, std::string>;
	using MaybeBuffer = tl::expected<VkBuffer, std::string>;
	using MaybeBuffers = tl::expected<std::vector<VkBuffer>, std::string>;
	using MaybeMemoryPropertyIndex = tl::expected<uint32_t, std::string>;
	using MaybeDeviceMemory = tl::expected<VkDeviceMemory, std::string>;
	using MaybeCommandPool = tl::expected<VkCommandPool, std::string>;
	using MaybeCommandBuffers = tl::expected<std::vector<VkCommandBuffer>, std::string>;
	
	MaybeShaderModule create_shader_module(VkDevice logicalDevice, std::string const & shaderFile);
	void destroy_debug_report_callback(VkInstance instance, VkDebugReportCallbackEXT vkDebugCallback);
	MaybeBuffer create_buffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage);
	MaybeBuffers create_buffers(VkDevice device, std::vector<VkDeviceSize> const & sizes, std::vector<VkBufferUsageFlags> const & usages);
	MaybeMemoryPropertyIndex get_supported_memory_property_index(VkPhysicalDevice physicalDevice,
	                                                             uint32_t supportedMemoryTypeBits,
	                                                             VkMemoryPropertyFlags desiredMemoryFlags);
	MaybeDeviceMemory allocate_device_memory(VkDevice device, VkDeviceSize allocationSize, uint32_t memoryTypeIndex);
	MaybeCommandPool create_command_pool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
	MaybeCommandBuffers allocate_command_buffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount = 1);
}