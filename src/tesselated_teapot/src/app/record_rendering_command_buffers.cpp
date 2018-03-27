#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData record_rendering_command_buffers(AppData appData)
	{
		assert(appData.renderPass);
		assert(appData.graphicsPipelineWireframe);
		assert(appData.positionsBuffer);
		assert(appData.commandBuffersWireframe.size() == appData.swapChainFramebuffers.size());
		assert(appData.commandBuffersSolid.size() == appData.swapChainFramebuffers.size());
		
		auto record_command_buffer = [&appData](VkCommandBuffer const commandBuffer, VkPipeline const pipeline, size_t ind)
		{
			vector<VkClearValue> const clearValues{{{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f}}};
			
			VkRenderPassBeginInfo const renderPassInfo{get_render_pass_begin_info(appData.renderPass, appData.swapChainFramebuffers[ind], appData.surfaceExtent, &clearValues)};
			
			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			
			vector<VkBuffer> const vertexBuffers{appData.positionsBuffer};
			vector<VkDeviceSize> const offsets{0, 0};
			vkCmdBindVertexBuffers(commandBuffer, 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
			
			vkCmdBindIndexBuffer(commandBuffer, appData.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			
			vector<VkDescriptorSet> const descSets{appData.descriptorSets[ind]};
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, appData.pipelineLayout, 0, static_cast<uint32_t>(descSets.size()), descSets.data(), 0, nullptr);
			
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(appData.teapotData.patches.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(commandBuffer);
		};
		
		for (size_t i = 0; i < appData.commandBuffersWireframe.size(); i++)
		{
			VkCommandBuffer const commandBuffer{appData.commandBuffersWireframe[i]};
			
			VkCommandBufferBeginInfo const beginInfo{get_command_buffer_begin_info()};
			
			if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
				return make_unexpected("failed to begin command buffer");
			
			record_command_buffer(commandBuffer, appData.graphicsPipelineWireframe, i);
			
			if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
				return make_unexpected("failed to end command buffer");
		}
		
		for (size_t i = 0; i < appData.commandBuffersSolid.size(); i++)
		{
			VkCommandBuffer const commandBuffer{appData.commandBuffersSolid[i]};
			
			VkCommandBufferBeginInfo const beginInfo{get_command_buffer_begin_info()};
			
			if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
				return make_unexpected("failed to begin command buffer");
			
			record_command_buffer(commandBuffer, appData.graphicsPipelineSolid, i);
			
			if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
				return make_unexpected("failed to end command buffer");
		}
		
		return appData;
	}
}