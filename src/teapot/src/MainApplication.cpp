#include "app/App.h"
#include "MainApplication.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>

MainApplication::MainApplication(){}

MainApplication::MainApplication(uint32_t const windowWidth, uint32_t const windowHeight, std::string const & appName) : MainApplication{}
{
	app::MaybeWindow const mbWindow{app::create_window(windowWidth, windowHeight, appName)};
	
	if (!mbWindow)
		throw std::runtime_error{mbWindow.error()};
	
	m_appData.window = *mbWindow;
	m_appData.layers.push_back("VK_LAYER_LUNARG_standard_validation");
	m_appData.instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	m_appData.deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	m_appData.debugCallback = &app::vulkan_debug_callback;
	
	glfwSetWindowUserPointer(m_appData.window, &m_appData);
	glfwSetKeyCallback(m_appData.window, &app::on_key_press);
	glfwSetWindowSizeLimits(m_appData.window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetFramebufferSizeCallback(m_appData.window, app::framebuffer_size_callback);

	app::MaybeAppData mbData{app::MaybeAppData{app::get_required_window_extensions(std::move(m_appData))}
	                         .and_then(app::create_instance)
	                         .and_then(app::create_surface)
	                         .and_then(app::get_physical_device)
	                         .map(app::prepare_device_features)
	                         .and_then(app::create_logical_device)
	                         .and_then(app::create_shader_modules)
	                         .and_then(app::create_debug_utils_messenger)
	                         .and_then(app::create_render_pass)
	                         .and_then(app::create_descriptor_set_layout)
	                         .and_then(app::create_pipeline_layout)
	                         .and_then(app::create_pipelines)
	                         .and_then(app::get_surface_capabilities)
	                         .map(app::get_surface_extent)
	                         .and_then(app::create_swap_chain)
	                         .and_then(app::create_frame_buffers)
	                         .map(app::get_device_qeues)
	                         .and_then(app::create_matrices_buffers)
	                         .and_then(app::create_positions_buffer)
	                         .and_then(app::create_index_buffer)
	                         .and_then(app::create_patches_buffer)};

	if (!mbData)
		throw std::runtime_error{mbData.error()};

	m_appData = std::move(*mbData);

	glfwSetWindowUserPointer(m_appData.window, &m_appData);
}

MainApplication::~MainApplication()
{
	app::clear(m_appData);
}

void MainApplication::run()
{
	while (!glfwWindowShouldClose(m_appData.window))
	{
		glfwPollEvents();
		render();
	}
}

void MainApplication::render()
{
	if(m_appData.framebufferResized)
	{
		m_appData.framebufferResized = false;
		
		app::MaybeAppData mbRenderData{app::resize_swap_chain(m_appData)};
		if(!mbRenderData)
			throw std::runtime_error{mbRenderData.error()};
		
		m_appData = std::move(*mbRenderData);
	}
	
	// TODO render cool stuff
}