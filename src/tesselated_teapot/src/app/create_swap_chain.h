#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

namespace app
{
	template<uint32_t ImgCount>
	MaybeAppData create_swap_chain(AppData appData)
	{
		assert(appData.surface);
		assert(appData.device);
	
		if (ImgCount < appData.surfaceCapabilities.minImageCount || ImgCount > appData.surfaceCapabilities.maxImageCount)
			return tl::make_unexpected("failed to support requested amount of swap chain images");
		
		VkSwapchainCreateInfoKHR const createInfo{vk_helpers::get_swap_chain_create_info(appData.surface,
		                                                                                 ImgCount,
		                                                                                 appData.surfaceFormat.format,
		                                                                                 appData.surfaceFormat.colorSpace,
		                                                                                 appData.surfaceExtent,
		                                                                                 appData.surfaceCapabilities.currentTransform,
		                                                                                 appData.surfacePresentMode)};
		
		if (vkCreateSwapchainKHR(appData.device, &createInfo, nullptr, &appData.swapChain) != VK_SUCCESS)
			return tl::make_unexpected("failed to create swap chain");
		
		return appData;
	}
}