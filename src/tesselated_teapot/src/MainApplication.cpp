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
	
	_appData.layers.push_back("VK_LAYER_LUNARG_standard_validation");
	_appData.instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	_appData.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	
	MaybeAppData maybeAppData{MaybeAppData{get_required_window_extensions(_appData)}
	                          .and_then(create_instance)
	                          .and_then(create_surface)
	                          .and_then(get_physical_device)
	                          .and_then(create_logical_device)
	                          .and_then(create_shader_modules)};
	
	if (!maybeAppData)
		throw runtime_error{maybeAppData.error()};
	
	_appData = *maybeAppData;
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