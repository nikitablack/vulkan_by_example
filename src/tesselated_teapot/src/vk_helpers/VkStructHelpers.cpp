#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk_helpers
{
	VkInstanceCreateInfo get_instance_create_info(vector<char const *> const * const extensions = nullptr,
	                                              vector<char const *> const * const layers = nullptr)
	{
		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.pApplicationInfo = nullptr;
		info.enabledLayerCount = (layers != nullptr) ? static_cast<uint32_t>(layers->size()) : 0;
		info.ppEnabledLayerNames = (layers != nullptr) ? layers->data() : nullptr;
		info.enabledExtensionCount = (extensions != nullptr) ? static_cast<uint32_t>(extensions->size()) : 0;
		info.ppEnabledExtensionNames = (extensions != nullptr) ? extensions->data() : nullptr;
		
		return info;
	}
	
	VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t const queueFamilyIndex, vector<float> const * const queuePriorities)
	{
		VkDeviceQueueCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.queueFamilyIndex = queueFamilyIndex;
		info.queueCount = (queuePriorities != nullptr) ? static_cast<uint32_t>(queuePriorities->size()) : 0;
		info.pQueuePriorities = (queuePriorities != nullptr) ? queuePriorities->data() : nullptr;
		
		return info;
	}
	
	VkDeviceCreateInfo get_device_create_info(vector<VkDeviceQueueCreateInfo> const * const queueCreateInfos,
	                                          vector<char const *> const * const deviceExtensions,
	                                          VkPhysicalDeviceFeatures const * const deviceFeatures)
	{
		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos->size());
		info.pQueueCreateInfos = queueCreateInfos->data();
		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;
		info.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions->size());
		info.ppEnabledExtensionNames = deviceExtensions->data();
		info.pEnabledFeatures = deviceFeatures;
		
		return info;
	}
	
	VkShaderModuleCreateInfo get_shader_module_create_info(vector<char> const * const shaderCode)
	{
		VkShaderModuleCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.codeSize = shaderCode->size();
		info.pCode = reinterpret_cast<uint32_t const *>(shaderCode->data());
		
		return info;
	}
}