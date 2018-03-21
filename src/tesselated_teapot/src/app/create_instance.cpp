#include "App.h"
#include "vk_helpers/VkStructHelpers.h"

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_instance(AppData appData)
	{
		VkInstanceCreateInfo const createInfo{get_instance_create_info(&appData.instanceExtensions, &appData.layers)};
		
		if (vkCreateInstance(&createInfo, nullptr, &appData.instance) != VK_SUCCESS)
			return make_unexpected("failed to create instance");
		
		return appData;
	}
}