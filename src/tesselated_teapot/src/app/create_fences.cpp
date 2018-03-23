#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_fences(AppData appData)
	{
		assert(appData.device);
		
		appData.commandBufferFences.resize(appData.swapChainFramebuffers.size());
		
		for(size_t i{0}; i < appData.commandBufferFences.size(); ++i)
		{
			VkFenceCreateInfo fenceInfo{get_fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT)};
			
			if(vkCreateFence(appData.device, &fenceInfo, nullptr, &appData.commandBufferFences[i]) != VK_SUCCESS)
				return make_unexpected("failed to create fence");
		}
		
		return appData;
	}
}