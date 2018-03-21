#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk_helpers
{
	VkInstanceCreateInfo get_instance_create_info(vector<char const *> const * const extensions = nullptr,
	                                              vector<char const *> const * const layers = nullptr)
	{
		VkInstanceCreateInfo vkInstCreateInfo{};
		vkInstCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vkInstCreateInfo.pNext = nullptr;
		vkInstCreateInfo.flags = 0;
		vkInstCreateInfo.pApplicationInfo = nullptr;
		vkInstCreateInfo.enabledLayerCount = (layers != nullptr) ? static_cast<uint32_t>(layers->size()) : 0;
		vkInstCreateInfo.ppEnabledLayerNames = (layers != nullptr) ? layers->data() : nullptr;
		vkInstCreateInfo.enabledExtensionCount = (extensions != nullptr) ? static_cast<uint32_t>(extensions->size()) : 0;
		vkInstCreateInfo.ppEnabledExtensionNames = (extensions != nullptr) ? extensions->data() : nullptr;
		
		return vkInstCreateInfo;
	}
	
	VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t const queueFamilyIndex, vector<float> const * const queuePriorities)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.pNext = nullptr;
		queueCreateInfo.flags = 0;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = (queuePriorities != nullptr) ? static_cast<uint32_t>(queuePriorities->size()) : 0;
		queueCreateInfo.pQueuePriorities = (queuePriorities != nullptr) ? queuePriorities->data() : nullptr;
		
		return queueCreateInfo;
	}
	
	VkDeviceCreateInfo get_device_create_info(vector<VkDeviceQueueCreateInfo> const * const queueCreateInfos,
	                                          vector<char const *> const * const deviceExtensions,
	                                          VkPhysicalDeviceFeatures const * const deviceFeatures)
	{
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos->size());
		createInfo.pQueueCreateInfos = queueCreateInfos->data();
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions->size());
		createInfo.ppEnabledExtensionNames = deviceExtensions->data();
		createInfo.pEnabledFeatures = deviceFeatures;
		
		return createInfo;
	}
	
	VkShaderModuleCreateInfo get_shader_module_create_info(vector<char> const * const shaderCode)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.codeSize = shaderCode->size();
		createInfo.pCode = reinterpret_cast<uint32_t const *>(shaderCode->data());
		
		return createInfo;
	}
}