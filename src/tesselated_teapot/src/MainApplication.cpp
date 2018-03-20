#include "app/App.h"
#include "MainApplication.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>

using namespace app;
using namespace std;

namespace
{
	void onKeyPress(GLFWwindow * const window, int const key, int const /*scancode*/, int const action, int const /*mods*/)
	{
		[[maybe_unused]] AppData * const appData{static_cast<AppData *>(glfwGetWindowUserPointer(window))};
		
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
}

MainApplication::MainApplication(uint32_t const windowWidth, uint32_t const windowHeight, std::string const & appName)
{
	GLFWwindow * const window{create_window(windowWidth, windowHeight, appName)};
	
	if (window == nullptr)
		throw runtime_error{"failed to create window"};
	
	_appData.window = window;
	
	glfwSetWindowUserPointer(window, &_appData);
	glfwSetKeyCallback(window, &onKeyPress);
}

MainApplication::~MainApplication()
{
	glfwTerminate();
}

void MainApplication::run()
{
	while (!glfwWindowShouldClose(_appData.window))
	{
		glfwPollEvents();
		render();
	}
}

void MainApplication::render()
{
	// TODO render cool stuff
}