#include "app/App.h"

#include <cassert>

namespace app
{

AppData get_device_qeues(AppData data)
{
	assert(data.device);
	
	vkGetDeviceQueue(data.device, data.graphicsFamilyQueueIndex, 0, &data.graphicsQueue);
	vkGetDeviceQueue(data.device, data.presentFamilyQueueIndex, 0, &data.presentQueue);
	
	return data;
}

} // namespace app