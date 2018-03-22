#include "details/Details.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeCreateDeviceLocalBufferData create_device_local_buffer(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		
		MaybeBuffer const mbBuffer{create_buffer(bufferData.device, bufferData.dataSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferData.localDeviceBufferUsage)};
		if (!mbBuffer)
			return make_unexpected(mbBuffer.error());
		
		bufferData.deviceLocalBufferRaii = VkRaii<VkBuffer>{*mbBuffer, getBufferDeleter(bufferData.device)};
		
		return move(bufferData);
	}
}