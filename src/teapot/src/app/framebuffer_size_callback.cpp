#include "app/App.h"

#include "GLFW/glfw3.h"

#include <cassert>

namespace app
{

void framebuffer_size_callback(GLFWwindow * const window, int const /*width*/, int const /*height*/)
{
	AppData * const data{static_cast<AppData *>(glfwGetWindowUserPointer(window))};

	assert(data);
	
	data->framebufferResized = true;
}

} // namespace app