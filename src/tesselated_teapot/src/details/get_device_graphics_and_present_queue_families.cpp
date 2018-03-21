#include "details/Details.h"

using namespace std;
using namespace tl;

namespace details
{
	MaybeQueueFamilies get_device_graphics_and_present_queue_families(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
	{
		uint32_t queueFamilyCount{0};
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		
		vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		
		for (size_t i{0}; i < queueFamilies.size(); ++i)
		{
			VkQueueFamilyProperties& queueFamily{queueFamilies[i]};
			
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				VkBool32 presentSupported{VK_FALSE};
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, static_cast<uint32_t>(i), surface, &presentSupported);
				
				if (presentSupported)
					return pair<uint32_t, uint32_t>{static_cast<uint32_t>(i), static_cast<uint32_t>(i)};
			}
		}
		
		int graphicsQueueIndex{-1};
		for (size_t i{0}; i < queueFamilies.size(); ++i)
		{
			VkQueueFamilyProperties& queueFamily{queueFamilies[i]};
			
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsQueueIndex = static_cast<int>(i);
				break;
			}
		}
		
		if (graphicsQueueIndex == -1)
			return make_unexpected("failed to find graphics queue");
		
		int presentQueueIndex{-1};
		for (size_t i{ 0 }; i < queueFamilies.size(); ++i)
		{
			VkQueueFamilyProperties& queueFamily{queueFamilies[i]};
			
			if (queueFamily.queueCount > 0)
			{
				VkBool32 presentSupported{VK_FALSE};
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, static_cast<uint32_t>(i), surface, &presentSupported);
				
				if (presentSupported)
				{
					presentQueueIndex = static_cast<int>(i);
					break;
				}
			}
		}
		
		if (presentQueueIndex == -1)
			return make_unexpected("failed to find present queue");
		
		return make_pair(static_cast<uint32_t>(graphicsQueueIndex), static_cast<uint32_t>(presentQueueIndex));
	}
}