#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData find_device_local_buffer_memory_type(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.deviceLocalBufferRaii.get());
		assert(bufferData.physicalDevice);
		
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(bufferData.device, bufferData.deviceLocalBufferRaii.get(), &memRequirements);
		
		MaybeMemoryPropertyIndex const mbMemPropIndex{get_supported_memory_property_index(bufferData.physicalDevice,
		                                                                                  memRequirements.memoryTypeBits,
		                                                                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};
		if (!mbMemPropIndex)
			return make_unexpected(mbMemPropIndex.error());
		
		bufferData.deviceLocalBufferMemoryTypeIndex = *mbMemPropIndex;
		
		return move(bufferData);
	}
}