#include "app/helpers/VkStructsHelpers.h"

#include <cassert>

namespace app::helpers
{

VkInstanceCreateInfo get_instance_create_info(std::vector<char const *> const * const extensions, std::vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
{
	VkInstanceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pApplicationInfo = applicationInfo;
	info.enabledLayerCount = (layers) ? static_cast<uint32_t>(layers->size()) : 0;
	info.ppEnabledLayerNames = (layers) ? layers->data() : nullptr;
	info.enabledExtensionCount = (extensions) ? static_cast<uint32_t>(extensions->size()) : 0;
	info.ppEnabledExtensionNames = (extensions) ? extensions->data() : nullptr;
	
	return info;
}

VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t const queueFamilyIndex, std::vector<float> const * const queuePriorities)
{
	assert(queuePriorities);
	assert(!queuePriorities->empty());

	VkDeviceQueueCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.queueFamilyIndex = queueFamilyIndex;
	info.queueCount = static_cast<uint32_t>(queuePriorities->size());
	info.pQueuePriorities = queuePriorities->data();

	return info;
}

VkDeviceCreateInfo get_device_create_info(std::vector<VkDeviceQueueCreateInfo> const * const queueCreateInfos, VkPhysicalDeviceFeatures const * const deviceFeatures, std::vector<char const *> const * const deviceExtensions)
{
	assert(queueCreateInfos);
	assert(!queueCreateInfos->empty());

	VkDeviceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos->size());
	info.pQueueCreateInfos = queueCreateInfos->data();
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = nullptr;
	info.enabledExtensionCount = deviceExtensions ? static_cast<uint32_t>(deviceExtensions->size()) : 0;
	info.ppEnabledExtensionNames = deviceExtensions ? deviceExtensions->data() : nullptr;
	info.pEnabledFeatures = deviceFeatures;

	return info;
}

} // namespace app::helpers