#include "details/Details.h"

#include <algorithm>

using namespace std;
using namespace tl;

namespace details
{
	MaybeSurfaceFormat get_device_surface_format(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
	{
		uint32_t formatsCount{0};
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr) != VK_SUCCESS)
			return make_unexpected("failed to get physical device surface formats");
		
		vector<VkSurfaceFormatKHR> formats(formatsCount);
		if (formatsCount == 0 || vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, formats.data()) != VK_SUCCESS)
			return make_unexpected("failed to get physical device surface formats");
		
		if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
			return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		
		if (auto const it = find_if(formats.begin(), formats.end(), [](VkSurfaceFormatKHR const f) { return f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; }); it != formats.end())
			return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		
		return formats[0];
	}
}