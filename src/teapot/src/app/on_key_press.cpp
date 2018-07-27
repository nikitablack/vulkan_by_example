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
		if(appData->tessLevel < appData->physicalDeviceProperties.limits.maxTessellationGenerationLevel)
			appData->tessLevel += 0.1f;
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if(appData->tessLevel > 1)
			appData->tessLevel -= 0.1f;
	}
	else if (key == GLFW_KEY_SPACE && (action == GLFW_RELEASE))
	{
		appData->currRenderingStyle = appData->currRenderingStyle == AppData::RenderingStyle::Wireframe ? AppData::RenderingStyle::Solid : AppData::RenderingStyle::Wireframe;
	}
}

} // namespace app