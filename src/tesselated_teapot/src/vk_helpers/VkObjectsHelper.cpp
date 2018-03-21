#include "vk_helpers/VkObjectsHelper.h"
#include "vk_helpers/VkStructHelpers.h"

#include <fstream>

using namespace std;
using namespace tl;

namespace
{
	using MaybeShaderData = tl::expected<std::vector<char>, std::string>;
	
	MaybeShaderData load_shader(string const & fileName)
	{
		ifstream file{fileName, ios::ate | ios::binary};
		
		if (!file.is_open())
			return make_unexpected("failed to open shader file");
		
		size_t const fileSize{static_cast<size_t>(file.tellg())};
		vector<char> buffer(fileSize);
		
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		
		file.close();
		
		if (!file)
			return make_unexpected("failed to read shader file");
		
		if (buffer.empty() || buffer.size() % 4 != 0)
			return make_unexpected("failed to read shader file");
		
		return buffer;
	}
}

namespace vk_helpers
{
	MaybeShaderModule create_shader_module(VkDevice const logicalDevice, string const & shaderFile)
	{
		MaybeShaderData const shaderData{load_shader(shaderFile)};
		if(!shaderData)
			return make_unexpected(shaderData.error());
		
		VkShaderModuleCreateInfo createInfo{get_shader_module_create_info(&(*shaderData))};
		
		VkShaderModule shaderModule{VK_NULL_HANDLE};
		if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			return make_unexpected("failed to create shader module");
		
		return shaderModule;
	}
	
	void destroy_debug_report_callback(VkInstance const instance, VkDebugReportCallbackEXT const vkDebugCallback)
	{
		PFN_vkDestroyDebugReportCallbackEXT const func{reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"))};
		if (func == nullptr)
			return;
		
		func(instance, vkDebugCallback, nullptr);
	}
}