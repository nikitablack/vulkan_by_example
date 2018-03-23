#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData allocate_descriptor_sets(AppData appData)
	{
		assert(appData.descriptorSetLayout);
		assert(appData.device);
		
		vector<VkDescriptorSetLayout> const setLayouts{appData.swapChainFramebuffers.size(), appData.descriptorSetLayout};
		VkDescriptorSetAllocateInfo const allocateInfo{get_descriptor_set_allocate_info(appData.descriptorPool, &setLayouts)};
		
		appData.descriptorSets.resize(appData.swapChainFramebuffers.size());
		if (vkAllocateDescriptorSets(appData.device, &allocateInfo, appData.descriptorSets.data()) != VK_SUCCESS)
			return make_unexpected("failed to allocate descriptor set");
		
		return appData;
	}
}