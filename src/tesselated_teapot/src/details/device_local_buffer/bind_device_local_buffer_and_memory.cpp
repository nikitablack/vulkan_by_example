#include "details/Details.h"

#include <cassert>

using namespace std;
using namespace tl;

namespace details
{
	MaybeCreateDeviceLocalBufferData bind_device_local_buffer_and_memory(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.deviceLocalBufferRaii.get());
		assert(bufferData.deviceLocalDeviceMemoryRaii.get());
		
		if (vkBindBufferMemory(bufferData.device, bufferData.deviceLocalBufferRaii.get(), bufferData.deviceLocalDeviceMemoryRaii.get(), 0) != VK_SUCCESS)
			return make_unexpected("failed to bind buffer memory");
		
		return move(bufferData);
	}
}