#include "App.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace app
{
	AppData get_required_window_extensions(AppData appData)
	{
		uint32_t glfwExtensionCount{0};
		char const * const * const glfwExtensions{glfwGetRequiredInstanceExtensions(&glfwExtensionCount)};
		
		for (uint32_t i{0}; i < glfwExtensionCount; ++i)
			appData.instanceExtensions.push_back(glfwExtensions[i]);
		
		return appData;
	}
}