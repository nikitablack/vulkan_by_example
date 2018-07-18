#include "app/App.h"

namespace app
{

AppData prepare_device_features(AppData data)
{
	data.physicalDeviceFeatures.tessellationShader = VK_TRUE;
	data.physicalDeviceFeatures.fillModeNonSolid = VK_TRUE;
	
	return data;
}

} // namespace app