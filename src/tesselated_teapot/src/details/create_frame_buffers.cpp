#include "vk_helpers/VkStructHelpers.h"
#include "Details.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeSwapChainResources create_frame_buffers(SwapChainImagesResourcesData resourcesData)
	{
		assert(resourcesData.device);
		assert(resourcesData.renderPass);
		
		resourcesData.framebuffers.resize(resourcesData.images.size());
		for(size_t i{0}; i < resourcesData.images.size(); ++i)
		{
			vector<VkImageView> const imageViews{resourcesData.imageViews[i]};
			
			VkFramebufferCreateInfo const framebufferInfo{get_frame_buffer_create_info(resourcesData.renderPass, resourcesData.width, resourcesData.height, &imageViews)};
			
			if (vkCreateFramebuffer(resourcesData.device, &framebufferInfo, nullptr, &resourcesData.framebuffers[i]) != VK_SUCCESS)
				return make_unexpected("failed to create framebuffer");
		}
		
		return resourcesData;
	}
}