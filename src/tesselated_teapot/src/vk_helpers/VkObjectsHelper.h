#pragma once

#include "tl/expected.hpp"

#include <string>
#include <vulkan/vulkan.hpp>

namespace vk_helpers
{
	using MaybeShaderModule = tl::expected<VkShaderModule, std::string>;
	
	MaybeShaderModule create_shader_module(VkDevice logicalDevice, std::string const & shaderFile);
	void destroy_debug_report_callback(VkInstance instance, VkDebugReportCallbackEXT vkDebugCallback);
}