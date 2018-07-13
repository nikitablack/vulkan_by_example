#include "app/App.h"

#include "GLFW/glfw3.h"

#include <cassert>

namespace app
{

void framebuffer_size_callback(GLFWwindow * const window, int const /*width*/, int const /*height*/)
{
	AppData * const appData{static_cast<AppData *>(glfwGetWindowUserPointer(window))};

	assert(appData);
	
	appData->framebufferResized = true;
}

} // namespace app