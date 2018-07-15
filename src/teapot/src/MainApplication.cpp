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
	
	glfwSetWindowUserPointer(m_appData.window, &m_appData);
	glfwSetKeyCallback(m_appData.window, &app::on_key_press);
	glfwSetWindowSizeLimits(m_appData.window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetFramebufferSizeCallback(m_appData.window, app::framebuffer_size_callback);

	app::MaybeAppData mbData{app::MaybeAppData{app::get_required_window_extensions(std::move(m_appData))}
		.and_then(app::create_instance)
		.and_then(app::create_surface)
		.and_then(app::get_physical_device)
		.and_then(app::create_logical_device)
		.and_then(app::create_shader_modules)};

	if (!mbData)
		throw std::runtime_error{mbData.error()};

	m_appData = std::move(*mbData);

	glfwSetWindowUserPointer(m_appData.window, &m_appData);
}

MainApplication::~MainApplication()
{
	glfwTerminate();
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
	// TODO render cool stuff
}