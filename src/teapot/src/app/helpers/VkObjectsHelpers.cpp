#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"

#include <cassert>
#include <stdexcept>

namespace app::helpers
{

VkInstance create_instance(std::vector<char const *> const * const extensions, std::vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
{
	VkInstanceCreateInfo const createInfo{get_instance_create_info(extensions, layers, applicationInfo)};
	
	VkInstance instance{VK_NULL_HANDLE};
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error{"failed to create instance"};
	
	return instance;
}

std::vector<VkPhysicalDevice> get_physical_devices(VkInstance const instance)
{
	assert(instance);

	uint32_t deviceCount{0};
	if (vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
		throw std::runtime_error{"failed to find GPUs with Vulkan support"};

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	if (vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
		throw std::runtime_error{"failed to find GPUs with Vulkan support"};

	return physicalDevices;
}

std::vector<VkSurfaceFormatKHR> get_physical_devices_surface_formats(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);

	uint32_t formatsCount{0};
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error{"failed to get physical device surface formats"};

	std::vector<VkSurfaceFormatKHR> formats(formatsCount);
	if (formatsCount == 0 || vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, formats.data()) != VK_SUCCESS)
		throw std::runtime_error{"failed to get physical device surface formats"};

	return formats;
}

std::vector<VkPresentModeKHR> get_physical_device_surface_present_modes(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);

	uint32_t presentModesCount{0};
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error{"failed to get physical device surface present modes"};

	std::vector<VkPresentModeKHR> presentModes(presentModesCount);
	if (presentModesCount == 0 || vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data()) != VK_SUCCESS)
		throw std::runtime_error{"failed to get physical device surface present modes"};

	return presentModes;
}

VkDevice create_device(VkPhysicalDevice const physicalDevice, std::vector<uint32_t> const * const queueIndices, std::vector<std::vector<float>> const * const queuePriorities, VkPhysicalDeviceFeatures const * const features, std::vector<char const *> const * const extensions)
{
	assert(physicalDevice);
	assert(queueIndices);
	assert(!queueIndices->empty());
	assert(queuePriorities);
	assert(queueIndices->size() == queuePriorities->size());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	queueCreateInfos.reserve(queueIndices->size());

	for (size_t i{0}; i < queueIndices->size(); ++i)
		queueCreateInfos.push_back(get_device_queue_create_info(queueIndices->at(i), &queuePriorities->at(i)));

	VkDeviceCreateInfo const createInfo{get_device_create_info(&queueCreateInfos, features, extensions)};

	VkDevice device{VK_NULL_HANDLE};
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		throw std::runtime_error{"failed to create logical device"};

	return device;
}

} // namespace app::helpers