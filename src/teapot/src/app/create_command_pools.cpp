#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_command_pools(AppData data)
{
	assert(data.device);
	
	helpers::MaybeCommandPool const mbRenderCommandPool{helpers::create_command_pool(data.device, data.graphicsFamilyQueueIndex)};
	if (!mbRenderCommandPool)
		return tl::make_unexpected(mbRenderCommandPool.error());
	
	data.renderCommandPool = *mbRenderCommandPool;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(data.renderCommandPool), "render command pool");
	
	helpers::MaybeCommandPool const mbPushConstantsCommandPool{helpers::create_command_pool(data.device, data.graphicsFamilyQueueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)};
	if (!mbPushConstantsCommandPool)
		return tl::make_unexpected(mbPushConstantsCommandPool.error());
	
	data.pushConstantsCommandPool = *mbPushConstantsCommandPool;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(data.pushConstantsCommandPool), "push constant command pool");
	
	helpers::MaybeCommandPool const mbUpdateMatricesCommandPool{helpers::create_command_pool(data.device, data.graphicsFamilyQueueIndex, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)};
	if (!mbUpdateMatricesCommandPool)
		return tl::make_unexpected(mbUpdateMatricesCommandPool.error());
	
	data.updateMatricesCommandPool = *mbUpdateMatricesCommandPool;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(data.updateMatricesCommandPool), "update matrices command pool");
	
	return data;
}

} // namespace app