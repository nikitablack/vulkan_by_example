#include "App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_logical_device(AppData appData)
	{
		assert(appData.physicalDevice);
	
		vector<float> const queuePriorities{1.0f};
		
		vector<VkDeviceQueueCreateInfo> queueCreateInfos{get_device_queue_create_info(appData.graphicsFamilyQueueIndex, &queuePriorities)};
		
		if (appData.graphicsFamilyQueueIndex != appData.presentFamilyQueueIndex)
			queueCreateInfos.push_back(get_device_queue_create_info(appData.presentFamilyQueueIndex, &queuePriorities));
		
		VkDeviceCreateInfo const createInfo{ get_device_create_info(&queueCreateInfos, &appData.deviceExtensions, &appData.physicalDeviceFeatures) };
		
		if (vkCreateDevice(appData.physicalDevice, &createInfo, nullptr, &appData.device) != VK_SUCCESS)
			return make_unexpected("failed to create logical device");
		
		return appData;
	}
}

