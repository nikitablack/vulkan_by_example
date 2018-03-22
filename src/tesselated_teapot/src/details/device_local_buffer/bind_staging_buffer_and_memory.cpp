#include "details/Details.h"

#include <cassert>

using namespace std;
using namespace tl;

namespace details
{
	MaybeCreateDeviceLocalBufferData bind_staging_buffer_and_memory(CreateDeviceLocalBufferData bufferData)
	{
		assert(bufferData.device);
		assert(bufferData.stagingBufferRaii.get());
		assert(bufferData.stagingDeviceMemoryRaii.get());
		
		if (vkBindBufferMemory(bufferData.device, bufferData.stagingBufferRaii.get(), bufferData.stagingDeviceMemoryRaii.get(), 0) != VK_SUCCESS)
			return make_unexpected("failed to bind buffer memory");
		
		return move(bufferData);
	}
}