#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_logical_device(AppData data)
{
	assert(data.physicalDevice);
	
	std::vector<uint32_t> queueIndices{data.graphicsFamilyQueueIndex};
	std::vector<std::vector<float>> queueNumAndPriorities{{1.0f}};
	
	if(data.graphicsFamilyQueueIndex != data.presentFamilyQueueIndex)
	{
		queueIndices.push_back(data.presentFamilyQueueIndex);
		queueNumAndPriorities.push_back({1.0f});
	}
	
	helpers::MaybeDevice const mbDevice{helpers::create_device(data.physicalDevice, &queueIndices, &queueNumAndPriorities, &data.physicalDeviceFeatures, &data.deviceExtensions)};
	if(!mbDevice)
		return tl::make_unexpected(mbDevice.error());
	
	data.device = *mbDevice;
	
	return data;
}

} // namespace app