#include "details/Details.h"

#include <algorithm>

using namespace std;
using namespace tl;

namespace details
{
	MaybePresentMode get_device_surface_present_mode(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
	{
		uint32_t presentModesCount{0};
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr) != VK_SUCCESS)
			return make_unexpected("failed to get physical device surface present modes");
		
		vector<VkPresentModeKHR> presentModes(presentModesCount);
		if (presentModesCount == 0 || vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data()) != VK_SUCCESS)
			return make_unexpected("failed to get physical device surface present modes");
		
		if (auto const it = find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR); it != presentModes.end())
			return VK_PRESENT_MODE_MAILBOX_KHR;
		
		if (auto const it = find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_IMMEDIATE_KHR); it != presentModes.end())
			return VK_PRESENT_MODE_IMMEDIATE_KHR;
	
		return VK_PRESENT_MODE_FIFO_KHR;
	}
}