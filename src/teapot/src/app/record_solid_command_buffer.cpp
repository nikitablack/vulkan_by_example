#include "helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData record_solid_command_buffer(AppData data)
{
	assert(data.renderPass);
	assert(data.solidPipeline);
	assert(data.positionsBuffer);
	assert(data.solidCommandBuffers.size() == data.swapChainFramebuffers.size());
	
	for (size_t i = 0; i < data.solidCommandBuffers.size(); i++)
	{
		VkCommandBuffer const commandBuffer{data.solidCommandBuffers[i]};
		
		VkCommandBufferBeginInfo const beginInfo{helpers::get_command_buffer_begin_info(0)};
		
		if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			return tl::make_unexpected("failed to begin command buffer");
		
		std::vector<VkClearValue> const clearValues{{0.0f, 0.5f, 0.0f, 1.0f}};
		
		VkRenderPassBeginInfo const renderPassInfo{helpers::get_render_pass_begin_info(data.renderPass, data.swapChainFramebuffers[i], {0, 0, data.surfaceExtent}, &clearValues)};
		
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		VkViewport const viewport{0.0f, 0.0f, static_cast<float>(data.surfaceExtent.width), static_cast<float>(data.surfaceExtent.height), 0.0f, 1.0f};
		VkRect2D const scissor{VkOffset2D{0, 0}, VkExtent2D{data.surfaceExtent.width, data.surfaceExtent.height}};
		
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, data.solidPipeline);
		
		VkDeviceSize const offset{0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &data.positionsBuffer, &offset);
		
		vkCmdBindIndexBuffer(commandBuffer, data.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, data.pipelineLayout, 0, 1, &data.descriptorSet, 0, nullptr);
		
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(data.teapotData.patches.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffer);
		
		if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			return tl::make_unexpected("failed to end command buffer");
	}
	
	return data;
}

}