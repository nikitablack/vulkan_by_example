#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData create_device_local_device_memory(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.deviceLocalBufferRaii.get());
		
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(bufferData.device, bufferData.deviceLocalBufferRaii.get(), &memRequirements);
		
		MaybeDeviceMemory const mbDeviceMemory{allocate_device_memory(bufferData.device, memRequirements.size, bufferData.deviceLocalBufferMemoryTypeIndex)};
		if (!mbDeviceMemory)
			return make_unexpected(mbDeviceMemory.error());
		
		bufferData.deviceLocalDeviceMemoryRaii = VkRaii<VkDeviceMemory>{*mbDeviceMemory, getDeviceMemoryDeleter(bufferData.device)};
		
		return move(bufferData);
	}
}