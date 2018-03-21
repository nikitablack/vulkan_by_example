#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_debug_report_callback(AppData appData)
	{
		assert(appData.debugCallbackPtr);
		assert(appData.instance);
	
		PFN_vkCreateDebugReportCallbackEXT const func{reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(appData.instance, "vkCreateDebugReportCallbackEXT"))};
		if (func == nullptr)
			return make_unexpected("failed to get debug report callback extension");
		
		VkDebugReportCallbackCreateInfoEXT const createInfo{get_debug_report_callback_createInfo(appData.debugCallbackPtr)};
		
		if (func(appData.instance, &createInfo, nullptr, &appData.debugCallback) != VK_SUCCESS)
			return make_unexpected("failed to create debug report callback");
		
		return appData;
	}
}