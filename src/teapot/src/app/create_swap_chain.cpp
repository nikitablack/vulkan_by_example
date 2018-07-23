#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_swap_chain(AppData appData)
{
	assert(appData.surface);
	assert(appData.device);

	constexpr uint32_t imgCount{3};
	
	if (imgCount < appData.surfaceCapabilities.minImageCount || imgCount > appData.surfaceCapabilities.maxImageCount)
		return tl::make_unexpected("failed to support requested amount of swap chain images");
	
	helpers::MaybeSwapchain const mbSwapChain{helpers::create_swap_chain(appData.device, appData.surface, imgCount, appData.surfaceFormat.format, appData.surfaceFormat.colorSpace, appData.surfaceExtent, appData.surfaceCapabilities.currentTransform, appData.surfacePresentMode, appData.swapChain)};
	
	if(!mbSwapChain)
		return tl::make_unexpected(mbSwapChain.error());
	
	appData.swapChain = *mbSwapChain;
	
	return appData;
}

}