#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_debug_utils_messenger(AppData data)
{
	assert(data.instance);
	assert(data.debugCallback);
	
	constexpr VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT};
	constexpr VkDebugUtilsMessageTypeFlagsEXT const messageType{VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT};
	
	helpers::MaybeDebugUtilsMessenger const mbMessenger{helpers::create_debug_utils_messenger(data.instance, messageSeverity, messageType, data.debugCallback)};
	if(!mbMessenger)
		return tl::make_unexpected(mbMessenger.error());
	
	data.debugUtilsMessenger = *mbMessenger;
	
	return data;
}

} // namespace app