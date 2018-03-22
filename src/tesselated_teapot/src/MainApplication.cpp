#include "app/App.h"
#include "app/create_swap_chain.h"
#include "vk_helpers/VkObjectsHelper.h"
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
	
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT const /*flags*/,
	                                             VkDebugReportObjectTypeEXT const /*objType*/,
	                                             uint64_t const /*obj*/,
	                                             size_t const /*location*/,
	                                             int32_t const /*code*/,
	                                             char const * const layerPrefix,
	                                             char const * const msg,
	                                             void * const /*userData*/)
	{
		cerr << "layer " << layerPrefix << ": " << msg << endl;
		
		return VK_FALSE;
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
	_appData.debugCallbackPtr = &debugCallback;
	
	MaybeAppData maybeAppData{MaybeAppData{get_required_window_extensions(_appData)}
	                          .and_then(create_instance)
	                          .and_then(create_surface)
	                          .and_then(get_physical_device)
	                          .and_then(create_logical_device)
	                          .and_then(create_shader_modules)
	                          .and_then(create_debug_report_callback)
	                          .and_then(create_render_pass)
	                          .and_then(create_descriptor_set_layout)
	                          .and_then(create_pipeline_layout)
	                          .and_then(create_graphics_pipelines)
	                          .and_then(create_swap_chain<3>)
	                          .and_then(create_frame_buffers)
	                          .and_then(create_matrices_buffers)
	                          .map(get_device_qeues)
	                          .and_then(create_positions_buffer)
	                          .and_then(create_index_buffer)
	                          .and_then(create_patches_buffer)};
	
	if (!maybeAppData)
		throw runtime_error{maybeAppData.error()};
	
	_appData = *maybeAppData;
}

MainApplication::~MainApplication()
{
	clear(_appData);
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