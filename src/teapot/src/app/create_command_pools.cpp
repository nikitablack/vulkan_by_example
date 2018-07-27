#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_command_pools(AppData data)
{
	assert(data.device);
	
	helpers::MaybeCommandPool const mbStaticCommandPool{helpers::create_command_pool(data.device, data.graphicsFamilyQueueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)};
	if (!mbStaticCommandPool)
		return tl::make_unexpected(mbStaticCommandPool.error());
	
	data.staticCommandPool = *mbStaticCommandPool;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(data.staticCommandPool), "static command pool");
	
	helpers::MaybeCommandPool const mbDynamicCommandPool{helpers::create_command_pool(data.device, data.graphicsFamilyQueueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)};
	if (!mbDynamicCommandPool)
		return tl::make_unexpected(mbDynamicCommandPool.error());
	
	data.dynamicCommandPool = *mbDynamicCommandPool;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(data.dynamicCommandPool), "dynamic command pool");
	
	return data;
}

} // namespace app