#include "vk_helpers/VkObjectsHelper.h"
#include "vk_helpers/VkStructHelpers.h"

#include <algorithm>
#include <fstream>

using namespace std;
using namespace tl;

namespace
{
	using MaybeShaderData = tl::expected<std::vector<char>, std::string>;
	
	MaybeShaderData load_shader(string const & fileName)
	{
		ifstream file{fileName, ios::ate | ios::binary};
		
		if (!file.is_open())
			return make_unexpected("failed to open shader file");
		
		size_t const fileSize{static_cast<size_t>(file.tellg())};
		vector<char> buffer(fileSize);
		
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		
		file.close();
		
		if (!file)
			return make_unexpected("failed to read shader file");
		
		if (buffer.empty() || buffer.size() % 4 != 0)
			return make_unexpected("failed to read shader file");
		
		return buffer;
	}
}

namespace vk_helpers
{
	MaybeShaderModule create_shader_module(VkDevice const logicalDevice, string const & shaderFile)
	{
		MaybeShaderData const shaderData{load_shader(shaderFile)};
		if(!shaderData)
			return make_unexpected(shaderData.error());
		
		VkShaderModuleCreateInfo createInfo{get_shader_module_create_info(&(*shaderData))};
		
		VkShaderModule shaderModule{VK_NULL_HANDLE};
		if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			return make_unexpected("failed to create shader module");
		
		return shaderModule;
	}
	
	void destroy_debug_report_callback(VkInstance const instance, VkDebugReportCallbackEXT const vkDebugCallback)
	{
		PFN_vkDestroyDebugReportCallbackEXT const func{reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"))};
		if (func == nullptr)
			return;
		
		func(instance, vkDebugCallback, nullptr);
	}
	
	MaybeBuffer create_buffer(VkDevice const device, VkDeviceSize const size, VkBufferUsageFlags const usage)
	{
		VkBufferCreateInfo const bufferInfo{get_buffer_create_info(size, usage)};
		
		VkBuffer buffer{VK_NULL_HANDLE};
		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			return make_unexpected("failed to create buffer");
		
		return buffer;
	}
	
	MaybeBuffers create_buffers(VkDevice const device, vector<VkDeviceSize> const & sizes, vector<VkBufferUsageFlags> const & usages)
	{
		assert(sizes.size() == usages.size());
		
		vector<VkBuffer> buffers(sizes.size());
		for (size_t i{ 0 }; i < buffers.size(); ++i)
		{
			MaybeBuffer const mbBuffer{create_buffer(device, sizes[i], usages[i])};
			if (!mbBuffer)
			{
				for_each(buffers.begin(), buffers.end(), [device](VkBuffer const buffer){vkDestroyBuffer(device, buffer, nullptr);});
				return make_unexpected(mbBuffer.error());
			}
			
			buffers[i] = *mbBuffer;
		}
		
		return buffers;
	}
	
	MaybeMemoryPropertyIndex get_supported_memory_property_index(VkPhysicalDevice const physicalDevice,
	                                                             uint32_t const supportedMemoryTypeBits,
	                                                             VkMemoryPropertyFlags const desiredMemoryFlags)
	{
		VkPhysicalDeviceMemoryProperties memProperties{};
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
		
		uint32_t memPropertyIndex{ 0 };
		for (uint32_t i{ 0 }; i < memProperties.memoryTypeCount; ++i)
		{
			if ((supportedMemoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & desiredMemoryFlags) == desiredMemoryFlags)
			{
				memPropertyIndex = i;
				return memPropertyIndex;
			}
		}
		
		return make_unexpected("failed to find memory property index");
	}
	
	MaybeDeviceMemory allocate_device_memory(VkDevice const device, VkDeviceSize const allocationSize, uint32_t const memoryTypeIndex)
	{
		VkMemoryAllocateInfo const allocateInfo{get_memory_allocate_info(allocationSize, memoryTypeIndex)};
		
		VkDeviceMemory deviceMemory{ VK_NULL_HANDLE };
		if (vkAllocateMemory(device, &allocateInfo, nullptr, &deviceMemory) != VK_SUCCESS)
			return make_unexpected("failed to allocate buffer memory");
		
		return deviceMemory;
	}
	
	MaybeCommandPool create_command_pool(VkDevice const device, uint32_t const queueFamilyIndex, VkCommandPoolCreateFlags const flags)
	{
		VkCommandPoolCreateInfo const commandPoolCreateInfo{get_command_pool_create_info(queueFamilyIndex, flags)};
		
		VkCommandPool commandPool{ VK_NULL_HANDLE };
		if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
			return make_unexpected("failed to create command pool");
		
		return commandPool;
	}
	
	MaybeCommandBuffers allocate_command_buffers(VkDevice const device, VkCommandPool const commandPool, uint32_t const commandBufferCount)
	{
		VkCommandBufferAllocateInfo const commandBufferAllocateInfo{get_command_buffer_allocate_info(commandPool, commandBufferCount)};
		
		vector<VkCommandBuffer> commandBuffers(commandBufferCount);
		if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS)
			return make_unexpected("failed to allocate command buffer");
		
		return {commandBuffers};
	}
}