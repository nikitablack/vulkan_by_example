#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeCommandBuffer allocate_synchronization_buffer(VkDevice const device, VkCommandPool const commandPool, VkBuffer const buffer, VkDeviceSize const size, VkDeviceSize const offset)
{
	assert(device);
	assert(commandPool);
	assert(buffer);
	
	helpers::MaybeCommandBuffers const mbCommandBuffers{helpers::allocate_command_buffers(device, commandPool, 1)};
	if (!mbCommandBuffers)
		return tl::make_unexpected(mbCommandBuffers.error());
	
	VkCommandBuffer const commandBuffer{(*mbCommandBuffers)[0]};
	
	VkCommandBufferBeginInfo const beginInfo{helpers::get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
	
	if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		return tl::make_unexpected("failed to begin command buffer");
	
	VkBufferMemoryBarrier const bufferMemoryBarrier{helpers::get_buffer_memory_barrier(buffer, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT, size, offset)};
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT, 0, 0, nullptr, 1, &bufferMemoryBarrier, 0, nullptr);
	
	if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		return tl::make_unexpected("failed to end command buffer");
	
	return commandBuffer;
}

} // namespace app