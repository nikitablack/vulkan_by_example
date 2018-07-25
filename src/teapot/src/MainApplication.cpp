#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"
#include "MainApplication.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>

namespace
{

std::vector<VkFence> waitFences{};
std::vector<VkSemaphore> waitSemaphores{};
std::vector<VkPipelineStageFlags> waitStages{};
std::vector<VkCommandBuffer> commandBuffers{};
std::vector<VkSemaphore> signalSemaphores{};
std::vector<VkSwapchainKHR> swapChains{};
std::vector<uint32_t> imageIndices{};

void clear_containers()
{
	waitFences.clear();
	waitSemaphores.clear();
	waitStages.clear();
	commandBuffers.clear();
	signalSemaphores.clear();
	swapChains.clear();
	imageIndices.clear();
}

} // namespace

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
	                         .and_then(app::create_patches_buffer)
	                         .and_then(app::create_command_pools)
	                         .and_then(app::allocate_command_buffers)
	                         .and_then(app::create_descriptor_pool)
	                         .and_then(app::allocate_descriptor_set)
	                         .map(app::update_descriptor_set)
	                         .and_then(app::create_semaphores)
	                         .and_then(app::create_command_buffer_fences)
	                         .and_then(app::record_wireframe_command_buffer)
	                         .and_then(app::record_solid_command_buffer)};

	if (!mbData)
		throw std::runtime_error{mbData.error()};

	m_appData = std::move(*mbData);

	glfwSetWindowUserPointer(m_appData.window, &m_appData);
}

MainApplication::~MainApplication()
{
	if(m_appData.device && vkDeviceWaitIdle(m_appData.device) != VK_SUCCESS)
		return; // don't throw in destructor
	
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
	clear_containers();
	
	uint32_t imageIndex{};
	if(vkAcquireNextImageKHR(m_appData.device, m_appData.swapChain, std::numeric_limits<uint64_t>::max(), m_appData.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
		throw std::runtime_error("failed to acquire swap chain image");
	
	waitFences.push_back(m_appData.commandBufferFences[imageIndex]);
	
	if(vkWaitForFences(m_appData.device, static_cast<uint32_t>(waitFences.size()), waitFences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS)
		throw std::runtime_error("failed to wait for fences");
	
	if(vkResetFences(m_appData.device, static_cast<uint32_t>(waitFences.size()), waitFences.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to reset fences");
	
	waitSemaphores.push_back(m_appData.imageAvailableSemaphore);
	waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	commandBuffers.push_back(m_appData.wireframeCommandBuffers[imageIndex]);
	signalSemaphores.push_back(m_appData.presentFinishedSemaphore);
	
	VkSubmitInfo const submitInfo{app::helpers::get_submit_info(&commandBuffers, &waitSemaphores, &waitStages, &signalSemaphores)};
	
	if(vkQueueSubmit(m_appData.graphicsQueue, 1, &submitInfo, m_appData.commandBufferFences[imageIndex]) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer");
	
	swapChains.push_back(m_appData.swapChain);
	imageIndices.push_back(imageIndex);
	
	VkPresentInfoKHR presentInfo{app::helpers::get_present_info(&swapChains, &imageIndices, &signalSemaphores)};
	
	VkResult const res{vkQueuePresentKHR(m_appData.presentQueue, &presentInfo)};
	if(res == VK_SUBOPTIMAL_KHR || res == VK_ERROR_OUT_OF_DATE_KHR || m_appData.framebufferResized)
	{
		m_appData.framebufferResized = false;
		
		app::MaybeAppData mbRenderData{app::resize_swap_chain(m_appData)};
		if(!mbRenderData)
			throw std::runtime_error{mbRenderData.error()};
		
		m_appData = std::move(*mbRenderData);
	}
	else if(res != VK_SUCCESS)
		throw std::runtime_error("failed to present");
}