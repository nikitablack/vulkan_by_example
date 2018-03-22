#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData create_copy_command_pool(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		
		MaybeCommandPool const mbCommandPool{create_command_pool(bufferData.device, bufferData.copyQueueFamilyIndex)};
		if (!mbCommandPool)
			return make_unexpected(mbCommandPool.error());
		
		bufferData.commandPoolRaii = VkRaii<VkCommandPool>{*mbCommandPool, getCommandPoolDeleter(bufferData.device)};
		
		return move(bufferData);
	}
}