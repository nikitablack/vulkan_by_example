#include "app/App.h"

#include <cassert>

using namespace std;
using namespace tl;

namespace app
{
	AppData get_device_qeues(AppData appData)
	{
		assert(appData.device);
		
		vkGetDeviceQueue(appData.device, appData.graphicsFamilyQueueIndex, 0, &appData.graphicsQueue);
		vkGetDeviceQueue(appData.device, appData.presentFamilyQueueIndex, 0, &appData.presentQueue);
		
		return appData;
	}
}