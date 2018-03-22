#include "details/Details.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>
#include <cstring>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData copy_data_to_staging_buffer(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.stagingDeviceMemoryRaii.get());
		assert(bufferData.data);
		
		void* data;
		if (vkMapMemory(bufferData.device, bufferData.stagingDeviceMemoryRaii.get(), 0, bufferData.dataSize, 0, &data) != VK_SUCCESS)
			return make_unexpected("failed to map vertex buffer memory");
		
		memcpy(data, bufferData.data, static_cast<size_t>(bufferData.dataSize));
		
		vector<VkMappedMemoryRange> const mappedMemoryRanges{get_mapped_memory_range(bufferData.stagingDeviceMemoryRaii.get(), bufferData.dataSize)};
		if (vkFlushMappedMemoryRanges(bufferData.device, static_cast<uint32_t>(mappedMemoryRanges.size()), mappedMemoryRanges.data()) != VK_SUCCESS)
		{
			vkUnmapMemory(bufferData.device, bufferData.stagingDeviceMemoryRaii.get());
			return make_unexpected("failed to flush mapped memory");
		}
		
		vkUnmapMemory(bufferData.device, bufferData.stagingDeviceMemoryRaii.get());
		
		return move(bufferData);
	}
}