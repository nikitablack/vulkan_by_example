#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>
#include <stdexcept>

namespace app
{

MaybeAppData create_logical_device(AppData appData)
{
	assert(appData.physicalDevice);
	
	std::vector<uint32_t> queueIndices{appData.graphicsFamilyQueueIndex};
	std::vector<std::vector<float>> queueNumAndPriorities{{1.0f}};
	
	if(appData.graphicsFamilyQueueIndex != appData.presentFamilyQueueIndex)
	{
		queueIndices.push_back(appData.presentFamilyQueueIndex);
		queueNumAndPriorities.push_back({1.0f});
	}
	
	helpers::MaybeDevice const mbDevice{helpers::create_device(appData.physicalDevice, &queueIndices, &queueNumAndPriorities, &appData.physicalDeviceFeatures, &appData.deviceExtensions)};
	if(!mbDevice)
		return tl::make_unexpected(mbDevice.error());
	
	appData.device = *mbDevice;
	
	return appData;
}

} // namespace app