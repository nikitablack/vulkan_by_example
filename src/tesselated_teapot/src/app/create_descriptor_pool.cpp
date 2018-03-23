#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_descriptor_pool(AppData appData)
	{
		assert(appData.device);
		
		vector<VkDescriptorPoolSize> const poolSizes{get_descriptor_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3),
		                                             get_descriptor_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 * 3)};
		VkDescriptorPoolCreateInfo const poolInfo{get_descriptor_pool_create_info(static_cast<uint32_t>(appData.swapChainFramebuffers.size()), &poolSizes)};
		
		if (vkCreateDescriptorPool(appData.device, &poolInfo, nullptr, &appData.descriptorPool) != VK_SUCCESS)
			return make_unexpected("failed to create descriptor pool");
		
		return appData;
	}
}