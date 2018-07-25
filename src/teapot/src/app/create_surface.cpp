#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <cassert>

namespace app
{

MaybeAppData create_surface(AppData data)
{
	assert(data.instance);
	assert(data.window);
	
	if (glfwCreateWindowSurface(data.instance, data.window, nullptr, &data.surface) != VK_SUCCESS)
		return tl::make_unexpected("failed to create window surface");
	
	return data;
}

} // namespace app