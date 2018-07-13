#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"

#include <cassert>
#include <stdexcept>

using namespace std;
using namespace tl;

namespace app::helpers
{

MaybeInstance create_instance(vector<char const *> const * const extensions, std::vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
{
VkInstanceCreateInfo const createInfo{get_instance_create_info(extensions, layers, applicationInfo)};

VkInstance instance{VK_NULL_HANDLE};
if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
return make_unexpected("failed to create instance");

return instance;
}

MaybePhysicalDevices get_physical_devices(VkInstance const instance)
{
	assert(instance);
	
	uint32_t deviceCount{0};
	if(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
		return make_unexpected("failed to find GPUs with Vulkan support");
	
	vector<VkPhysicalDevice> physicalDevices(deviceCount);
	if(vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
		return make_unexpected("failed to find GPUs with Vulkan support");
	
	return physicalDevices;
}

MaybePhysicalDevicesSurfaceFormats get_physical_devices_surface_formats(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);
	
	uint32_t formatsCount{0};
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, nullptr) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface formats");
	
	vector<VkSurfaceFormatKHR> formats(formatsCount);
	if (formatsCount == 0 || vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatsCount, formats.data()) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface formats");
	
	return formats;
}

MaybePhysicalDevicesSurfacePresentModes get_physical_device_surface_present_modes(VkPhysicalDevice const physicalDevice, VkSurfaceKHR const surface)
{
	assert(physicalDevice);
	assert(surface);
	
	uint32_t presentModesCount{0};
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface present modes");
	
	vector<VkPresentModeKHR> presentModes(presentModesCount);
	if (presentModesCount == 0 || vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes.data()) != VK_SUCCESS)
		return make_unexpected("failed to get physical device surface present modes");
	
	return presentModes;
}

MaybeDevice create_device(VkPhysicalDevice const physicalDevice, vector<uint32_t> const * const queueIndices, vector<vector<float>> const * const queuePriorities, VkPhysicalDeviceFeatures const * const features, vector<char const *> const * const extensions)
{
	assert(physicalDevice);
	assert(queueIndices);
	assert(!queueIndices->empty());
	assert(queuePriorities);
	assert(queueIndices->size() == queuePriorities->size());
	
	vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	queueCreateInfos.reserve(queueIndices->size());
	
	for(size_t i{0}; i < queueIndices->size(); ++i)
		queueCreateInfos.push_back(get_device_queue_create_info(queueIndices->at(i), &queuePriorities->at(i)));
	
	VkDeviceCreateInfo const createInfo{get_device_create_info(&queueCreateInfos, features, extensions)};
	
	VkDevice device{VK_NULL_HANDLE};
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		return make_unexpected("failed to create logical device");
	
	return device;
}

MaybeShaderModule create_shader_module(VkDevice const device, vector<char> const * const shaderCode)
{
	assert(device);
	
	VkShaderModuleCreateInfo createInfo{get_shader_module_create_info(shaderCode)};
	
	VkShaderModule shaderModule{VK_NULL_HANDLE};
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		return make_unexpected("failed to create shader module");
	
	return shaderModule;
}

} // namespace app::helpers