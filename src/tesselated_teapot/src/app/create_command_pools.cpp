#include "app/App.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_command_pools(AppData appData)
	{
		assert(appData.device);
		
		{
			MaybeCommandPool const mbCommandPool{create_command_pool(appData.device, appData.graphicsFamilyQueueIndex)};
			if (!mbCommandPool)
				return make_unexpected(mbCommandPool.error());
			
			appData.commandPoolRender = *mbCommandPool;
		}
		
		{
			MaybeCommandPool const mbCommandPool{create_command_pool(appData.device, appData.graphicsFamilyQueueIndex,
			                                                         VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)};
			if (!mbCommandPool)
				return make_unexpected(mbCommandPool.error());
			
			appData.commandPoolPushConstants = *mbCommandPool;
		}
		
		{
			MaybeCommandPool const mbCommandPool{create_command_pool(appData.device, appData.graphicsFamilyQueueIndex,
			                                                         VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)};
			if (!mbCommandPool)
				return make_unexpected(mbCommandPool.error());
			
			appData.commandPoolMatrices = *mbCommandPool;
		}
		
		return appData;
	}
}