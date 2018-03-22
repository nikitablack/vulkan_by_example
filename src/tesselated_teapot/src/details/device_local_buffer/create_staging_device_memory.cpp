#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData create_staging_device_memory(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.stagingBufferRaii.get());
		
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(bufferData.device, bufferData.stagingBufferRaii.get(), &memRequirements);
		
		MaybeDeviceMemory const mbDeviceMemory{allocate_device_memory(bufferData.device, memRequirements.size, bufferData.stagingBufferMemoryTypeIndex)};
		if (!mbDeviceMemory)
			return make_unexpected(mbDeviceMemory.error());
		
		bufferData.stagingDeviceMemoryRaii = VkRaii<VkDeviceMemory>{*mbDeviceMemory, getDeviceMemoryDeleter(bufferData.device)};
		
		return move(bufferData);
	}
}