#include "details/vk_raii/VkRaii.h"

#include "tl/expected.hpp"

#include <string>

namespace details
{
	struct CreateDeviceLocalBufferData
	{
		VkDevice device{ VK_NULL_HANDLE };
		VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
		VkDeviceSize dataSize{};
		void* data{ nullptr };
		VkBufferUsageFlags localDeviceBufferUsage{};
		VkBufferUsageFlags localDeviceBufferAccessMask{};
		VkBufferUsageFlags localDeviceBufferStageMask{};
		uint32_t copyQueueFamilyIndex{};
		VkQueue copyQueue{ VK_NULL_HANDLE };
		
		VkRaii<VkBuffer> stagingBufferRaii{};
		uint32_t stagingBufferMemoryTypeIndex{};
		VkRaii<VkDeviceMemory> stagingDeviceMemoryRaii{};
		
		VkRaii<VkBuffer> deviceLocalBufferRaii{};
		uint32_t deviceLocalBufferMemoryTypeIndex{};
		VkRaii<VkDeviceMemory> deviceLocalDeviceMemoryRaii{};
		
		VkRaii<VkCommandPool> commandPoolRaii{};
		VkCommandBuffer commandBuffer{ VK_NULL_HANDLE };
	};
	
	using MaybeCreateDeviceLocalBufferData = tl::expected<CreateDeviceLocalBufferData, std::string>;
	
	MaybeCreateDeviceLocalBufferData create_staging_buffer(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData find_staging_buffer_memory_type(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData create_staging_device_memory(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData bind_staging_buffer_and_memory(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData copy_data_to_staging_buffer(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData create_device_local_buffer(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData find_device_local_buffer_memory_type(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData create_device_local_device_memory(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData bind_device_local_buffer_and_memory(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData create_copy_command_pool(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData allocate_command_buffer(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData copy_buffer(CreateDeviceLocalBufferData bufferData);
	MaybeCreateDeviceLocalBufferData create_device_local_buffer_with_data(CreateDeviceLocalBufferData bufferData);
}