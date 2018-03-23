#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_present_semaphores(AppData appData)
	{
		assert(appData.device);
		
		VkSemaphoreCreateInfo const semaphoreInfo{get_semaphore_create_info()};
		
		if(vkCreateSemaphore(appData.device, &semaphoreInfo, nullptr, &appData.imageAvailableSemaphore) != VK_SUCCESS)
			return make_unexpected("failed to create semaphore");
		
		if(vkCreateSemaphore(appData.device, &semaphoreInfo, nullptr, &appData.renderFinishedSemaphore) != VK_SUCCESS)
			return make_unexpected("failed to create semaphore");
		
		return appData;
	}
}