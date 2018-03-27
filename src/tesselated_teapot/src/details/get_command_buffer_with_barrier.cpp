#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"
#include "vk_helpers/VkStructHelpers.h"

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCommandBuffer get_command_buffer_with_barrier(VkDevice const device, VkCommandPool const commandPool, VkBuffer const buffer, VkDeviceSize const size)
	{
		MaybeCommandBuffers const mbCommandBuffers{allocate_command_buffers(device, commandPool, 1)};
		if (!mbCommandBuffers)
			return make_unexpected(mbCommandBuffers.error());
		
		VkCommandBuffer const commandBuffer{(*mbCommandBuffers)[0]};
		
		VkCommandBufferBeginInfo const beginInfo{get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
		
		if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			return make_unexpected("failed to begin command buffer");
		
		vector<VkBufferMemoryBarrier> const bufferMemoryBarriers{get_buffer_memory_barrier(buffer, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT, size)};
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
		
		if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			return make_unexpected("failed to end command buffer");
		
		return commandBuffer;
	}
}