#include "vk_helpers/VkStructHelpers.h"
#include "Details.h"

#include <cassert>

using namespace tl;
using namespace vk_helpers;

namespace details
{
	MaybeSwapChainResources create_image_views(SwapChainImagesResourcesData resourcesData)
	{
		assert(resourcesData.device);
		
		resourcesData.imageViews.resize(resourcesData.images.size());
		for (size_t i{ 0 }; i < resourcesData.images.size(); ++i)
		{
			VkImageViewCreateInfo const createInfo{get_image_view_create_info(resourcesData.images[i], resourcesData.format.format, VK_IMAGE_ASPECT_COLOR_BIT)};
			
			if (vkCreateImageView(resourcesData.device, &createInfo, nullptr, &resourcesData.imageViews[i]) != VK_SUCCESS)
				return make_unexpected("failed to create image view");
		}
		
		return resourcesData;
	}
}