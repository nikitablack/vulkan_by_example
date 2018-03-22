#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData find_staging_buffer_memory_type(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.stagingBufferRaii.get());
		
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(bufferData.device, bufferData.stagingBufferRaii.get(), &memRequirements);
		
		MaybeMemoryPropertyIndex const mbMemPropIndex{get_supported_memory_property_index(bufferData.physicalDevice,
		                                                                                  memRequirements.memoryTypeBits,
		                                                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)};
		if (!mbMemPropIndex)
			return make_unexpected(mbMemPropIndex.error());
		
		bufferData.stagingBufferMemoryTypeIndex = *mbMemPropIndex;
		
		return move(bufferData);
	}
}