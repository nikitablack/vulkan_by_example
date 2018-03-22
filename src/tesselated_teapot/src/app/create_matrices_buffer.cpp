#include "app/App.h"
#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace details;
using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_matrices_buffers(AppData appData)
	{
		assert(appData.device);
		assert(appData.physicalDevice);
		assert(!appData.swapChainFramebuffers.empty());
		
		uint32_t const numFrameBuffers{static_cast<uint32_t>(appData.swapChainFramebuffers.size())};
		uint32_t const buffersCount{numFrameBuffers * 3};
		
		MaybeBuffers const buffers{create_buffers(appData.device, vector<VkDeviceSize>(buffersCount, sizeof(float) * 16), vector<VkBufferUsageFlags>(buffersCount, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT))};
		if (!buffers)
			return make_unexpected(buffers.error());
		
		vector<VkBuffer> const allBuffers{*buffers};
		
		appData.projMatricesBuffers.insert(appData.projMatricesBuffers.begin(), allBuffers.begin(), allBuffers.begin() + numFrameBuffers);
		appData.viewMatricesBuffers.insert(appData.viewMatricesBuffers.begin(), allBuffers.begin() + numFrameBuffers, allBuffers.begin() + numFrameBuffers * 2);
		appData.modelMatricesBuffers.insert(appData.modelMatricesBuffers.begin(), allBuffers.begin() + numFrameBuffers * 2, allBuffers.end());
		
		{
			VkMemoryRequirements memRequirements{};
			vkGetBufferMemoryRequirements(appData.device, appData.projMatricesBuffers[0], &memRequirements);
			
			MaybeMemoryPropertyIndex const mbMemPropIndex{get_supported_memory_property_index(appData.physicalDevice,
			                                                                            memRequirements.memoryTypeBits,
			                                                                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)};
			
			if (!mbMemPropIndex)
				return make_unexpected(mbMemPropIndex.error());
			
			VkDeviceSize const totalSize{calculate_total_buffers_size(appData.device, allBuffers)};
			
			MaybeDeviceMemory const mbDeviceMemory{allocate_device_memory(appData.device, totalSize, *mbMemPropIndex)};
			if (!mbDeviceMemory)
				return make_unexpected(mbDeviceMemory.error());
			
			appData.matricesDeviceMemory = *mbDeviceMemory;
		}
		
		{
			VkDeviceSize offset{0};
			for (size_t i{0}; i < allBuffers.size(); ++i)
			{
				VkMemoryRequirements memRequirements{};
				vkGetBufferMemoryRequirements(appData.device, allBuffers[i], &memRequirements);
				
				if (offset % memRequirements.alignment > 0)
					offset = offset - (offset % memRequirements.alignment) + memRequirements.alignment;
				
				if (vkBindBufferMemory(appData.device, allBuffers[i], appData.matricesDeviceMemory, offset) != VK_SUCCESS)
					return make_unexpected("failed to bind buffer memory");
				
				if(i < numFrameBuffers)
					appData.projMatricesOffsets.push_back(offset);
				else if(i < numFrameBuffers * 2)
					appData.viewMatricesOffsets.push_back(offset);
				else
					appData.modelMatricesOffsets.push_back(offset);
				
				offset += memRequirements.size;
			}
		}
		
		return appData;
	}
}