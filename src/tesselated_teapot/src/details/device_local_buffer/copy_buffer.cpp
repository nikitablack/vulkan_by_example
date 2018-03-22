#include "details/Details.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData copy_buffer(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.commandBuffer);
		assert(bufferData.stagingBufferRaii.get());
		assert(bufferData.deviceLocalBufferRaii.get());
		
		VkCommandBufferBeginInfo const beginInfo{get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
		if (vkBeginCommandBuffer(bufferData.commandBuffer, &beginInfo) != VK_SUCCESS)
			return make_unexpected("failed to begin command buffer");
		
		{
			// not necessary - "The vkQueueSubmit command automatically guarantees that host writes flushed to VK_ACCESS_HOST_WRITE_BIT are made available if they were flushed before the command executed"
			// https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkAccessFlagBits.html
			vector<VkBufferMemoryBarrier> const bufferMemoryBarriers{get_buffer_memory_barrier(bufferData.stagingBufferRaii.get(), VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, bufferData.dataSize)};
			vkCmdPipelineBarrier(bufferData.commandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
		}
		
		{
			vector<VkBufferCopy> const copyRegions{get_buffer_copy(bufferData.dataSize)};
			vkCmdCopyBuffer(bufferData.commandBuffer, bufferData.stagingBufferRaii.get(), bufferData.deviceLocalBufferRaii.get(), static_cast<uint32_t>(copyRegions.size()), copyRegions.data());
		}
		
		{
			vector<VkBufferMemoryBarrier> const bufferMemoryBarriers{get_buffer_memory_barrier(bufferData.deviceLocalBufferRaii.get(), VK_ACCESS_TRANSFER_WRITE_BIT, bufferData.localDeviceBufferAccessMask, bufferData.dataSize)};
			vkCmdPipelineBarrier(bufferData.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, bufferData.localDeviceBufferStageMask, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
		}
		
		if (vkEndCommandBuffer(bufferData.commandBuffer) != VK_SUCCESS)
			return make_unexpected("failed to end command buffer");
		
		vector<VkCommandBuffer> const commandBuffers{ bufferData.commandBuffer };
		vector<VkSubmitInfo> submitInfos{get_submit_info(&commandBuffers)};
		
		if (vkQueueSubmit(bufferData.copyQueue, static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), VK_NULL_HANDLE) != VK_SUCCESS)
			return make_unexpected("failed to submit queue");
		
		if (vkDeviceWaitIdle(bufferData.device) != VK_SUCCESS)
			return make_unexpected("failed to wait device idle");
		
		return move(bufferData);
	}
}