#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include "Dense"

using namespace details;
using namespace std;
using namespace tl;
using namespace vk_helpers;

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace
{
	Matrix4 lookAt(Eigen::RowVector3f const & eye, Eigen::RowVector3f const & at, Eigen::RowVector3f const & up)
	{
		Eigen::RowVector3f const zaxis{(at - eye).normalized()};
		Eigen::RowVector3f const xaxis{(up.cross(zaxis)).normalized()};
		Eigen::RowVector3f const yaxis{zaxis.cross(xaxis)};
		
		Matrix4 pmat;
		pmat << xaxis.x(), yaxis.x(), zaxis.x(), 0.0f,
				xaxis.y(), yaxis.y(), zaxis.y(), 0.0f,
				xaxis.z(), yaxis.z(), zaxis.z(), 0.0f,
				xaxis.dot(-eye), yaxis.dot(-eye), zaxis.dot(-eye), 1.0f;
		
		return pmat;
	}
}

namespace app
{
	MaybeCommandBuffer update_view_matrix(AppData const & appData, uint32_t bufferIndex)
	{
		static VkDeviceSize dataSize{sizeof(float) * 16};
		
		Matrix4 mat{lookAt({0.0f, 10.0f, 5.5f}, {0.0f, 0.0f, 1.5f}, {0.0f, 0.0f, -1.0f})};
		
		void* data;
		if(vkMapMemory(appData.device, appData.matricesDeviceMemory, appData.viewMatricesOffsets[bufferIndex], dataSize, 0, &data) != VK_SUCCESS)
			return make_unexpected("failed to map vertex buffer memory");
		
		memcpy(data, mat.data(), sizeof(float) * 16);
		
		vector<VkMappedMemoryRange> const mappedMemoryRanges{get_mapped_memory_range(appData.matricesDeviceMemory, dataSize, appData.viewMatricesOffsets[bufferIndex])};
		if (vkFlushMappedMemoryRanges(appData.device, static_cast<uint32_t>(mappedMemoryRanges.size()), mappedMemoryRanges.data()) != VK_SUCCESS)
			return make_unexpected("failed to flush mapped memory");
		
		vkUnmapMemory(appData.device, appData.matricesDeviceMemory);
		
		MaybeCommandBuffer const mbCommandBuffer{get_command_buffer_with_barrier(appData.device, appData.commandPoolMatrices, appData.viewMatricesBuffers[bufferIndex], dataSize)};
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