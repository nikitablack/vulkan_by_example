#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace
{

app::MaybeStaticBufferData create_staging_buffer(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	
	helpers::MaybeBuffer const mbBuffer{helpers::create_buffer(data.device, data.dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE)};
	if (!mbBuffer)
		return tl::make_unexpected(mbBuffer.error());
	
	data.stagingBufferRaii = helpers::VkRaii<VkBuffer>{*mbBuffer, helpers::get_buffer_deleter(data.device)};
	
	return std::move(data);
}

app::MaybeStaticBufferData find_staging_buffer_memory_type(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.stagingBufferRaii.get());
	
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(data.device, data.stagingBufferRaii.get(), &memRequirements);
	
	helpers::MaybeMemoryPropertyIndex const mbMemPropIndex{helpers::get_supported_memory_property_index(data.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)};
	if (!mbMemPropIndex)
		return tl::make_unexpected(mbMemPropIndex.error());
	
	data.stagingBufferMemoryTypeIndex = *mbMemPropIndex;
	
	return std::move(data);
}

app::MaybeStaticBufferData create_staging_device_memory(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.stagingBufferRaii.get());
	
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(data.device, data.stagingBufferRaii.get(), &memRequirements);
	
	helpers::MaybeDeviceMemory const mbDeviceMemory{helpers::allocate_device_memory(data.device, memRequirements.size, data.stagingBufferMemoryTypeIndex)};
	if (!mbDeviceMemory)
		return tl::make_unexpected(mbDeviceMemory.error());
	
	data.stagingDeviceMemoryRaii = helpers::VkRaii<VkDeviceMemory>{*mbDeviceMemory, helpers::get_device_memory_deleter(data.device)};
	
	return std::move(data);
}

app::MaybeStaticBufferData bind_staging_buffer_and_memory(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.stagingBufferRaii.get());
	assert(data.stagingDeviceMemoryRaii.get());
	
	if (vkBindBufferMemory(data.device, data.stagingBufferRaii.get(), data.stagingDeviceMemoryRaii.get(), 0) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind buffer memory");
	
	return std::move(data);
}

app::MaybeStaticBufferData copy_data_to_staging_buffer(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.stagingDeviceMemoryRaii.get());
	assert(data.data);
	
	void * mappedDataPtr{nullptr};
	if (vkMapMemory(data.device, data.stagingDeviceMemoryRaii.get(), 0, data.dataSize, 0, &mappedDataPtr) != VK_SUCCESS)
		return tl::make_unexpected("failed to map vertex buffer memory");
	
	memcpy(mappedDataPtr, data.data, static_cast<size_t>(data.dataSize));
	
	vkUnmapMemory(data.device, data.stagingDeviceMemoryRaii.get());
	
	return std::move(data);
}

app::MaybeStaticBufferData create_buffer(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	
	helpers::MaybeBuffer const mbBuffer{helpers::create_buffer(data.device, data.dataSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | data.localDeviceBufferUsage, VK_SHARING_MODE_EXCLUSIVE)};
	if (!mbBuffer)
		return tl::make_unexpected(mbBuffer.error());
	
	data.bufferRaii = helpers::VkRaii<VkBuffer>{*mbBuffer, helpers::get_buffer_deleter(data.device)};
	
	return std::move(data);
}

app::MaybeStaticBufferData find_buffer_memory_type(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.bufferRaii.get());
	assert(data.physicalDevice);
	
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(data.device, data.bufferRaii.get(), &memRequirements);
	
	helpers::MaybeMemoryPropertyIndex const mbMemPropIndex{helpers::get_supported_memory_property_index(data.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};
	if (!mbMemPropIndex)
		return tl::make_unexpected(mbMemPropIndex.error());
	
	data.bufferMemoryTypeIndex = *mbMemPropIndex;
	
	return std::move(data);
}

app::MaybeStaticBufferData create_device_memory(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.bufferRaii.get());
	
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(data.device, data.bufferRaii.get(), &memRequirements);
	
	helpers::MaybeDeviceMemory const mbDeviceMemory{helpers::allocate_device_memory(data.device, memRequirements.size, data.bufferMemoryTypeIndex)};
	if (!mbDeviceMemory)
		return tl::make_unexpected(mbDeviceMemory.error());
	
	data.deviceMemoryRaii = helpers::VkRaii<VkDeviceMemory>{*mbDeviceMemory, helpers::get_device_memory_deleter(data.device)};
	
	return std::move(data);
}

app::MaybeStaticBufferData bind_buffer_and_memory(app::StaticBufferData data)
{
	assert(data.device);
	assert(data.bufferRaii.get());
	assert(data.deviceMemoryRaii.get());
	
	if (vkBindBufferMemory(data.device, data.bufferRaii.get(), data.deviceMemoryRaii.get(), 0) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind buffer memory");
	
	return std::move(data);
}

app::MaybeStaticBufferData create_copy_command_pool(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	
	helpers::MaybeCommandPool const mbCommandPool{helpers::create_command_pool(data.device, data.copyQueueFamilyIndex)};
	if (!mbCommandPool)
		return tl::make_unexpected(mbCommandPool.error());
	
	data.commandPoolRaii = helpers::VkRaii<VkCommandPool>{*mbCommandPool, helpers::get_command_pool_deleter(data.device)};
	
	return std::move(data);
}

app::MaybeStaticBufferData allocate_command_buffer(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.commandPoolRaii.get());
	
	helpers::MaybeCommandBuffers const mbCommandBuffers{helpers::allocate_command_buffers(data.device, data.commandPoolRaii.get(), 1)};
	if (!mbCommandBuffers)
		return tl::make_unexpected(mbCommandBuffers.error());
	
	data.commandBuffer = (*mbCommandBuffers)[0];
	
	return std::move(data);
}

app::MaybeStaticBufferData copy_buffer(app::StaticBufferData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.commandBuffer);
	assert(data.stagingBufferRaii.get());
	assert(data.bufferRaii.get());
	
	VkCommandBufferBeginInfo const beginInfo{helpers::get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
	if (vkBeginCommandBuffer(data.commandBuffer, &beginInfo) != VK_SUCCESS)
		return tl::make_unexpected("failed to begin command buffer");
	
	{
		std::vector<VkBufferCopy> const copyRegions{helpers::get_buffer_copy(data.dataSize)};
		vkCmdCopyBuffer(data.commandBuffer, data.stagingBufferRaii.get(), data.bufferRaii.get(), static_cast<uint32_t>(copyRegions.size()), copyRegions.data());
	}
	
	{
		std::vector<VkBufferMemoryBarrier> const bufferMemoryBarriers{helpers::get_buffer_memory_barrier(data.bufferRaii.get(), VK_ACCESS_TRANSFER_WRITE_BIT, data.localDeviceBufferAccessMask, data.dataSize)};
		vkCmdPipelineBarrier(data.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, data.localDeviceBufferStageMask, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
	}
	
	if (vkEndCommandBuffer(data.commandBuffer) != VK_SUCCESS)
		return tl::make_unexpected("failed to end command buffer");
	
	std::vector<VkCommandBuffer> const commandBuffers{data.commandBuffer};
	std::vector<VkSubmitInfo> submitInfos{helpers::get_submit_info(&commandBuffers)};
	
	if (vkQueueSubmit(data.copyQueue, static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), VK_NULL_HANDLE) != VK_SUCCESS)
		return tl::make_unexpected("failed to submit queue");
	
	if (vkDeviceWaitIdle(data.device) != VK_SUCCESS)
		return tl::make_unexpected("failed to wait device idle");
	
	return std::move(data);
}

} // namespace

namespace app
{

MaybeStaticBufferData create_static_buffer(StaticBufferData data)
{
	MaybeStaticBufferData mbBufferData{create_staging_buffer(std::move(data))
	                                   .and_then(find_staging_buffer_memory_type)
	                                   .and_then(create_staging_device_memory)
	                                   .and_then(bind_staging_buffer_and_memory)
	                                   .and_then(copy_data_to_staging_buffer)
	                                   .and_then(create_buffer)
	                                   .and_then(find_buffer_memory_type)
	                                   .and_then(create_device_memory)
	                                   .and_then(bind_buffer_and_memory)
	                                   .and_then(create_copy_command_pool)
	                                   .and_then(allocate_command_buffer)
	                                   .and_then(copy_buffer)};
	
	return mbBufferData;
}

} // namespace app