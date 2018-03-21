#include "app/App.h"
#include "details/Details.h"

#include <cassert>

using namespace details;
using namespace std;
using namespace tl;

namespace app
{
	MaybeAppData create_frame_buffers(AppData appData)
	{
		assert(appData.device);
		assert(appData.swapChain);
		assert(appData.renderPass);
		
		SwapChainImagesResourcesData resourcesData{};
		resourcesData.device = appData.device;
		resourcesData.swapChain = appData.swapChain;
		resourcesData.renderPass = appData.renderPass;
		resourcesData.format = appData.surfaceFormat;
		resourcesData.width = appData.surfaceExtent.width;
		resourcesData.height = appData.surfaceExtent.height;
		
		MaybeSwapChainResources mbRes{get_swap_chain_images(resourcesData)
		                              .and_then(create_image_views)
		                              .and_then(details::create_frame_buffers)};
		
		if(!mbRes)
			return make_unexpected(mbRes.error());
		
		appData.swapChainFramebuffers = mbRes->framebuffers;
		appData.swapChainImageViews = mbRes->imageViews;
		
		return appData;
	}
}