#include "app/App.h"

#include "GLFW/glfw3.h"

#include <cassert>

namespace app
{

void on_key_press(GLFWwindow * const window, int const key, int const /*scancode*/, int const action, int const /*mods*/)
{
	AppData * const appData{static_cast<AppData *>(glfwGetWindowUserPointer(window))};
	
	assert(appData);

	if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// TODO increase tesselation level
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// TODO decrease tesselation level
	}
	else if (key == GLFW_KEY_SPACE && (action == GLFW_RELEASE))
	{
		// TODO switch wireframe/solid rendering
	}
}

} // namespace app