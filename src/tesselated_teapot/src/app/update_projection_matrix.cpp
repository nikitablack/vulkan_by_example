#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include "Dense"

#include <cmath>

using namespace details;
using namespace std;
using namespace tl;
using namespace vk_helpers;

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace
{
	Matrix4 perspective(float const fovy, float const aspect, float const zNear, float const zFar)
	{
		float const yScale{1.0f / tan(fovy / 2)};
		float const xScale{yScale / aspect};
		
		Matrix4 pmat{};
		pmat << xScale, 0, 0, 0,
				0, yScale, 0, 0,
				0, 0, zFar / (zFar - zNear), 1,
				0, 0, -zNear*zFar / (zFar - zNear), 0;
		
		return pmat;
	}
}

namespace app
{
	MaybeCommandBuffer update_projection_matrix(AppData const & appData, uint32_t bufferIndex)
	{
		static VkDeviceSize dataSize{sizeof(float) * 16};
		
		Matrix4 mat{perspective(45.0f *  3.14159265358979323846f / 180.0f, static_cast<float>(appData.surfaceExtent.width) / appData.surfaceExtent.height, 0.1f, 100.0f)};
		
		void* data;
		if(vkMapMemory(appData.device, appData.matricesDeviceMemory, appData.projMatricesOffsets[bufferIndex], dataSize, 0, &data) != VK_SUCCESS)
			return make_unexpected("failed to map vertex buffer memory");
		
		memcpy(data, mat.data(), sizeof(float) * 16);
		
		vector<VkMappedMemoryRange> const mappedMemoryRanges{get_mapped_memory_range(appData.matricesDeviceMemory, dataSize, appData.projMatricesOffsets[bufferIndex])};
		if (vkFlushMappedMemoryRanges(appData.device, static_cast<uint32_t>(mappedMemoryRanges.size()), mappedMemoryRanges.data()) != VK_SUCCESS)
			return make_unexpected("failed to flush mapped memory");
		
		vkUnmapMemory(appData.device, appData.matricesDeviceMemory);
		
		MaybeCommandBuffer const mbCommandBuffer{get_command_buffer_with_barrier(appData.device, appData.commandPoolMatrices, appData.projMatricesBuffers[bufferIndex], dataSize)};
		if(!mbCommandBuffer)
			return make_unexpected(mbCommandBuffer.error());
		
		VkCommandBuffer const commandBuffer{*mbCommandBuffer};
		
		static vector<VkCommandBuffer> commandBuffers(appData.swapChainFramebuffers.size(), VK_NULL_HANDLE);
		
		if(commandBuffers[bufferIndex] != VK_NULL_HANDLE)
			vkFreeCommandBuffers(appData.device, appData.commandPoolMatrices, 1, &commandBuffers[bufferIndex]);
		
		commandBuffers[bufferIndex] = commandBuffer;
		
		return commandBuffer;
	}
}