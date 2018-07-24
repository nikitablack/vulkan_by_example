#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_command_buffer_fences(AppData data)
{
	assert(data.device);
	
	data.commandBufferFences.clear();
	data.commandBufferFences.reserve(data.numConcurrentResources);
	
	for(size_t i{0}; i < data.numConcurrentResources; ++i)
	{
		helpers::MaybeFence const mbFence{helpers::create_fence(data.device, VK_FENCE_CREATE_SIGNALED_BIT)};
		if(!mbFence)
			return tl::make_unexpected(mbFence.error());
		
		data.commandBufferFences.push_back(*mbFence);
	}
	
	return data;
}

} // namespace app