#include "App.h"
#include "details/Details.h"

#include "GLFW/glfw3.h"

#include <cassert>
#include <algorithm>

using namespace details;
using namespace std;
using namespace tl;

namespace app
{
	MaybeAppData get_physical_device(AppData appData)
	{
		assert(appData.instance);
		assert(appData.surface);
	
		uint32_t deviceCount{0};
		if(vkEnumeratePhysicalDevices(appData.instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
			return make_unexpected("failed to find GPUs with Vulkan support");
		
		vector<VkPhysicalDevice> physicalDevices(deviceCount);
		if(vkEnumeratePhysicalDevices(appData.instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
			return make_unexpected("failed to find GPUs with Vulkan support");
		
		for(VkPhysicalDevice const d : physicalDevices)
		{
			MaybeSurfaceFormat const mbSurfaceFormat{get_device_surface_format(d, appData.surface)};
			if(!mbSurfaceFormat)
				continue;
			
			MaybePresentMode const mbPresentMode{get_device_surface_present_mode(d, appData.surface)};
			if(!mbPresentMode)
				continue;
			
			if(!check_device_suitability(d, appData.deviceExtensions))
				continue;
			
			MaybeQueueFamilies const mbQueueFamilies{get_device_graphics_and_present_queue_families(d, appData.surface)};
			if(!mbQueueFamilies)
				continue;
			
			VkSurfaceCapabilitiesKHR surfaceCapabilities{};
			if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(d, appData.surface, &surfaceCapabilities) != VK_SUCCESS)
				continue;
			
			appData.surfaceCapabilities = surfaceCapabilities;
			
			appData.physicalDevice = d;
			appData.graphicsFamilyQueueIndex = (*mbQueueFamilies).first;
			appData.presentFamilyQueueIndex = (*mbQueueFamilies).second;
			appData.surfaceFormat = *mbSurfaceFormat;
			appData.surfacePresentMode = *mbPresentMode;
			appData.surfaceExtent = get_surface_extent(appData.window, appData.surfaceCapabilities);
			
			vkGetPhysicalDeviceFeatures(appData.physicalDevice, &appData.physicalDeviceFeatures);
			vkGetPhysicalDeviceProperties(appData.physicalDevice, &appData.physicalDeviceProperties);
			
			return appData;
		}
		
		return make_unexpected("failed to find suitable device");
	}
}