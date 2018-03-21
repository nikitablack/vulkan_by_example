#include "App.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cassert>

using namespace std;
using namespace tl;

namespace app
{
	MaybeAppData create_surface(AppData appData)
	{
		assert(appData.instance);
		assert(appData.window);
	
		if (glfwCreateWindowSurface(appData.instance, appData.window, nullptr, &appData.surface) != VK_SUCCESS)
			return make_unexpected("failed to create window surface");
		
		return appData;
	}
}