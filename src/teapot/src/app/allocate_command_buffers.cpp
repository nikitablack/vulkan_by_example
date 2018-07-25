#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData allocate_command_buffers(AppData data)
{
	assert(data.device);
	assert(data.staticCommandPool);
	assert(data.dynamicCommandPool);
	
	helpers::MaybeCommandBuffers const mbWireframeCommandBuffers{helpers::allocate_command_buffers(data.device, data.staticCommandPool, data.numConcurrentResources)};
	if (!mbWireframeCommandBuffers)
		return tl::make_unexpected(mbWireframeCommandBuffers.error());
	
	data.wireframeCommandBuffers = *mbWireframeCommandBuffers;
	
	helpers::MaybeCommandBuffers const mbSolidCommandBuffers{helpers::allocate_command_buffers(data.device, data.staticCommandPool, data.numConcurrentResources)};
	if (!mbSolidCommandBuffers)
		return tl::make_unexpected(mbSolidCommandBuffers.error());
	
	data.solidCommandBuffers = *mbSolidCommandBuffers;
	
	helpers::MaybeCommandBuffers const mbPushConstantsCommandBuffers{helpers::allocate_command_buffers(data.device, data.dynamicCommandPool, data.numConcurrentResources)};
	if (!mbPushConstantsCommandBuffers)
		return tl::make_unexpected(mbPushConstantsCommandBuffers.error());
	
	data.pushConstantsCommandBuffers = *mbPushConstantsCommandBuffers;
	
	return data;
}

} // namespace app