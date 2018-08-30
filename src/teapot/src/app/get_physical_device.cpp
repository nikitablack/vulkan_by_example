#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <tuple>

using namespace std;

namespace {

using MaybeSurfaceFormat = tl::expected<VkSurfaceFormatKHR, string>;
using MaybePresentMode = tl::expected<VkPresentModeKHR, string>;
using MaybeQueueFamilies = tl::expected<tuple<uint32_t, uint32_t>, string>;

MaybeSurfaceFormat get_device_surface_format(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	app::helpers::MaybePhysicalDevicesSurfaceFormats const mbFormats{app::helpers::get_physical_devices_surface_formats(physicalDevice, surface)};
	if (!mbFormats)
		return tl::make_unexpected(mbFormats.error());
	
	vector<VkSurfaceFormatKHR> const formats{*mbFormats};

	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	
	if (auto const it = find_if(begin(formats), end(formats), [](VkSurfaceFormatKHR const f) { return f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; }); it != end(formats))
		return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	
	return formats[0];
}

MaybePresentMode get_device_surface_present_mode(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	app::helpers::MaybePhysicalDevicesSurfacePresentModes const mbPresentModes{app::helpers::get_physical_device_surface_present_modes(physicalDevice, surface)};
	if (!mbPresentModes)
		return tl::make_unexpected(mbPresentModes.error());
	
	vector<VkPresentModeKHR> const presentModes{*mbPresentModes};

	if (auto const it = find(begin(presentModes), end(presentModes), VK_PRESENT_MODE_MAILBOX_KHR); it != end(presentModes))
		return VK_PRESENT_MODE_MAILBOX_KHR;
	
	if (auto const it = find(begin(presentModes), end(presentModes), VK_PRESENT_MODE_IMMEDIATE_KHR); it != end(presentModes))
		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	
	return VK_PRESENT_MODE_FIFO_KHR;
}

bool check_required_device_extensions(VkPhysicalDevice const physicalDevice, vector<char const *> const & requiredExtensions)
{
	app::helpers::MaybeExtensionProperties mbExtensions{app::helpers::get_physical_device_device_extension_properties(physicalDevice)};
	if(!mbExtensions)
		return false;
	
	vector<VkExtensionProperties> const & availableExtensions{*mbExtensions};
	
	for (char const * element : requiredExtensions)
	{
		if (find_if(begin(availableExtensions), end(availableExtensions), [element](VkExtensionProperties const & extensionProp) { return strcmp(element, extensionProp.extensionName) == 0; }) == end(availableExtensions))
			return false;
	}
	
	return true;
}

bool check_device_suitability(VkPhysicalDevice const physicalDevice, vector<char const *> const & requiredExtensions)
{
	VkPhysicalDeviceProperties deviceProperties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	
	if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return false;
	
	VkPhysicalDeviceFeatures deviceFeatures{};
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	
	if (!deviceFeatures.tessellationShader)
		return false;
	
	if (deviceProperties.limits.maxTessellationPatchSize < 16)
		return false;
	
	if (!deviceFeatures.fillModeNonSolid)
		return false;
	
	if (!check_required_device_extensions(physicalDevice, requiredExtensions))
		return false;
	
	return true;
}

MaybeQueueFamilies get_device_graphics_and_present_queue_families(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	vector<VkQueueFamilyProperties> const queueFamilies{app::helpers::get_queue_family_properties(physicalDevice)};
	
	for (size_t i{0}; i < queueFamilies.size(); ++i)
	{
		VkQueueFamilyProperties queueFamily{queueFamilies[i]};
		
		if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			VkBool32 presentSupported{VK_FALSE};
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, static_cast<uint32_t>(i), surface, &presentSupported);
			
			if (presentSupported)
				return make_tuple(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
		}
	}
	
	int graphicsQueueIndex{-1};
	for (size_t i{0}; i < queueFamilies.size(); ++i)
	{
		VkQueueFamilyProperties queueFamily{queueFamilies[i]};
		
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphicsQueueIndex = static_cast<int>(i);
			break;
		}
	}
	
	if (graphicsQueueIndex == -1)
		return tl::make_unexpected("failed to find graphics queue");
	
	int presentQueueIndex{-1};
	for (size_t i{0}; i < queueFamilies.size(); ++i)
	{
		VkQueueFamilyProperties const queueFamily{queueFamilies[i]};
		
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
		return tl::make_unexpected("failed to find present queue");
	
	return make_tuple(static_cast<uint32_t>(graphicsQueueIndex), static_cast<uint32_t>(presentQueueIndex));
}

} // namespace

namespace app
{

MaybeAppData get_physical_device(AppData data)
{
	assert(data.instance);
	assert(data.surface);
	
	helpers::MaybePhysicalDevices const mbPhysicalDevices{helpers::get_physical_devices(data.instance)};
	if(!mbPhysicalDevices)
		return tl::make_unexpected(mbPhysicalDevices.error());
	
	vector<VkPhysicalDevice> const & physicalDevices{*mbPhysicalDevices};
	
	for(VkPhysicalDevice const d : physicalDevices)
	{
		if(!check_device_suitability(d, data.deviceExtensions))
			continue;
		
		MaybeSurfaceFormat const mbSurfaceFormat{get_device_surface_format(d, data.surface)};
		if(!mbSurfaceFormat)
			continue;
		
		MaybePresentMode const mbPresentMode{get_device_surface_present_mode(d, data.surface)};
		if(!mbPresentMode)
			continue;
		
		MaybeQueueFamilies const mbQueueFamilies{get_device_graphics_and_present_queue_families(d, data.surface)};
		if(!mbQueueFamilies)
			continue;
		
		data.physicalDevice = d;
		tie(data.graphicsFamilyQueueIndex, data.presentFamilyQueueIndex) = *mbQueueFamilies;
		data.surfaceFormat = *mbSurfaceFormat;
		data.surfacePresentMode = *mbPresentMode;
		
		vkGetPhysicalDeviceProperties(data.physicalDevice, &data.physicalDeviceProperties);
		
		return data;
	}
	
	return tl::make_unexpected("failed to find suitable device");
}

} // namespace app