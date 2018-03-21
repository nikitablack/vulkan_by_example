#include "Details.h"

#include <cassert>

using namespace tl;

namespace details
{
	MaybeSwapChainResources get_swap_chain_images(SwapChainImagesResourcesData resourcesData)
	{
		assert(resourcesData.device);
		assert(resourcesData.swapChain);
		
		uint32_t imageCount{ 0 };
		if (vkGetSwapchainImagesKHR(resourcesData.device, resourcesData.swapChain, &imageCount, nullptr) != VK_SUCCESS)
			return make_unexpected("failed to get swap chain images");
		
		resourcesData.images.resize(imageCount);
		if (vkGetSwapchainImagesKHR(resourcesData.device, resourcesData.swapChain, &imageCount, resourcesData.images.data()) != VK_SUCCESS)
			return make_unexpected("failed to get swap chain images");
		
		return resourcesData;
	}
}

