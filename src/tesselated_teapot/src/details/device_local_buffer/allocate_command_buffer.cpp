#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData allocate_command_buffer(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.commandPoolRaii.get());
		
		MaybeCommandBuffers const mbCommandBuffers{allocate_command_buffers(bufferData.device, bufferData.commandPoolRaii.get())};
		if (!mbCommandBuffers)
			return make_unexpected(mbCommandBuffers.error());
		
		bufferData.commandBuffer = (*mbCommandBuffers)[0];
		
		return move(bufferData);
	}
}