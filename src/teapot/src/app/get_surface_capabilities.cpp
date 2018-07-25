#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData get_surface_capabilities(AppData data)
{
	assert(data.physicalDevice);
	assert(data.surface);
	
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(data.physicalDevice, data.surface, &data.surfaceCapabilities) != VK_SUCCESS)
		return tl::make_unexpected("failed to fet physical device surface capabilities");
	
	return std::move(data);
}

} // app