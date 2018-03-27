#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include "Dense"

using namespace details;
using namespace std;
using namespace tl;
using namespace vk_helpers;

using Matrix4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;

namespace app
{
	MaybeCommandBuffer update_model_matrix(AppData const & appData, uint32_t bufferIndex)
	{
		static VkDeviceSize dataSize{sizeof(float) * 16};
		
		static uint32_t n{0};
		++n;
		
		Eigen::AngleAxis<float> const aa{n * 0.001f, Eigen::Vector3f{0.0f, 0.0f, 1.0f}};
		Eigen::Matrix3f const r{aa.toRotationMatrix()};
		
		Matrix4 mat{Matrix4::Identity()};
		mat.row(0) << r.row(0), 0;
		mat.row(1) << r.row(1), 0;
		mat.row(2) << r.row(2), 0;
		
		void* data;
		if(vkMapMemory(appData.device, appData.matricesDeviceMemory, appData.modelMatricesOffsets[bufferIndex], dataSize, 0, &data) != VK_SUCCESS)
			return make_unexpected("failed to map vertex buffer memory");
		
		memcpy(data, mat.data(), sizeof(float) * 16);
		
		vector<VkMappedMemoryRange> const mappedMemoryRanges{ get_mapped_memory_range(appData.matricesDeviceMemory, dataSize, appData.modelMatricesOffsets[bufferIndex]) };
		if (vkFlushMappedMemoryRanges(appData.device, static_cast<uint32_t>(mappedMemoryRanges.size()), mappedMemoryRanges.data()) != VK_SUCCESS)
			return make_unexpected("failed to flush mapped memory");
		
		vkUnmapMemory(appData.device, appData.matricesDeviceMemory);
		
		MaybeCommandBuffer const mbCommandBuffer{get_command_buffer_with_barrier(appData.device, appData.commandPoolMatrices, appData.modelMatricesBuffers[bufferIndex], dataSize)};
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