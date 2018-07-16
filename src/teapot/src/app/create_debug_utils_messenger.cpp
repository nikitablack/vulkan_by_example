#include "homm/render/vulkan/helpers/VkObjectsHelpers.h"
#include "homm/render/Render.h"

#include <cassert>

namespace homm::render::vulkan
{

MaybeRenderData create_debug_utils_messenger(RenderData data)
{
	assert(data.instance);
	assert(data.debugCallback);
	
	constexpr VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity{/*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | */VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT};
	constexpr VkDebugUtilsMessageTypeFlagsEXT const messageType{/*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | */VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT};
	
	helpers::MaybeDebugUtilsMessenger mbMessenger{helpers::create_debug_utils_messenger(data.instance, messageSeverity, messageType, data.debugCallback)};
	if(!mbMessenger)
		return tl::make_unexpected(mbMessenger.error());
	
	data.debugUtilsMessenger = *mbMessenger;
	
	return std::move(data);
}

} // namespace homm::render::vulkan