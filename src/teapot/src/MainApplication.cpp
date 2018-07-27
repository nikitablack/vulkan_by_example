#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"
#include "MainApplication.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>
#include <app/helpers/VkObjectsHelpers.h>

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
	
	m_numProjMatrixBuffersToUpdate = m_appData.numConcurrentResources;
	m_numViewMatrixBuffersToUpdate = m_appData.numConcurrentResources;
	m_numModelMatrixBuffersToUpdate = m_appData.numConcurrentResources;
	
	void * mappedPtr{nullptr};
	if(vkMapMemory(m_appData.device, m_appData.matricesDeviceMemory, 0, VK_WHOLE_SIZE, 0, &mappedPtr) != VK_SUCCESS)
		throw std::runtime_error{"failed to map uniform buffer memory"};
	m_matricesMemoryPtr = static_cast<char *>(mappedPtr);
}

MainApplication::~MainApplication()
{
	if(m_appData.device && vkDeviceWaitIdle(m_appData.device) != VK_SUCCESS)
		return; // don't throw in destructor
	
	if(m_matricesMemoryPtr)
		vkUnmapMemory(m_appData.device, m_appData.matricesDeviceMemory);
	
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
	
	{
		VkCommandBufferBeginInfo const beginInfo{app::helpers::get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
		
		if(vkBeginCommandBuffer(m_appData.pushConstantsCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin command buffer");
		
		vkCmdPushConstants(m_appData.pushConstantsCommandBuffers[imageIndex], m_appData.pipelineLayout, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, 0, sizeof(m_appData.tessLevel), &m_appData.tessLevel);
		
		if(vkEndCommandBuffer(m_appData.pushConstantsCommandBuffers[imageIndex]) != VK_SUCCESS)
			throw std::runtime_error("failed to end command buffer");
	}
	
	if(m_numProjMatrixBuffersToUpdate > 0)
	{
		app::MaybeCommandBuffer const mbUpdateProjMatrixCommandBuffer{app::get_update_project_matrix_command_buffer(m_appData.device, m_appData.dynamicCommandPool, m_appData.projMatrixBuffer, imageIndex, m_appData.numConcurrentResources, static_cast<float>(m_appData.surfaceExtent.width) / m_appData.surfaceExtent.height, m_matricesMemoryPtr + sizeof(float) * 16 * imageIndex)};
		if(!mbUpdateProjMatrixCommandBuffer)
			throw std::runtime_error(mbUpdateProjMatrixCommandBuffer.error());
		
		commandBuffers.push_back(*mbUpdateProjMatrixCommandBuffer);
		
		--m_numProjMatrixBuffersToUpdate;
	}
	
	if(m_numViewMatrixBuffersToUpdate > 0)
	{
		app::update_view_matrix(m_matricesMemoryPtr + m_appData.matrixBufferOffset + sizeof(float) * 16 * imageIndex);
		
		app::MaybeCommandBuffer const mbUpdateViewMatrixCommandBuffer{app::allocate_synchronization_buffer(m_appData.device, m_appData.dynamicCommandPool, m_appData.viewMatrixBuffer, sizeof(float) * 16, sizeof(float) * 16 * imageIndex)};
		if(!mbUpdateViewMatrixCommandBuffer)
			throw std::runtime_error(mbUpdateViewMatrixCommandBuffer.error());
		
		static std::vector<VkCommandBuffer> updateViewMatrixCommandBuffers(m_appData.numConcurrentResources, VK_NULL_HANDLE);
		
		if(updateViewMatrixCommandBuffers[imageIndex] != VK_NULL_HANDLE)
			vkFreeCommandBuffers(m_appData.device, m_appData.dynamicCommandPool, 1, &updateViewMatrixCommandBuffers[imageIndex]);
		
		updateViewMatrixCommandBuffers[imageIndex] = *mbUpdateViewMatrixCommandBuffer;
		
		commandBuffers.push_back(updateViewMatrixCommandBuffers[imageIndex]);
		
		--m_numViewMatrixBuffersToUpdate;
	}
	
	if(m_numModelMatrixBuffersToUpdate > 0)
	{
		static uint32_t n{0};
		++n;
		m_numModelMatrixBuffersToUpdate = m_appData.numConcurrentResources;
		
		app::update_model_matrix(n, m_matricesMemoryPtr + m_appData.matrixBufferOffset * 2 + sizeof(float) * 16 * imageIndex);
		
		app::MaybeCommandBuffer const mbUpdateModelMatrixCommandBuffer{app::allocate_synchronization_buffer(m_appData.device, m_appData.dynamicCommandPool, m_appData.modelMatrixBuffer, sizeof(float) * 16, sizeof(float) * 16 * imageIndex)};
		if(!mbUpdateModelMatrixCommandBuffer)
			throw std::runtime_error(mbUpdateModelMatrixCommandBuffer.error());
		
		static std::vector<VkCommandBuffer> updateModelMatrixCommandBuffers(m_appData.numConcurrentResources, VK_NULL_HANDLE);
		
		if(updateModelMatrixCommandBuffers[imageIndex] != VK_NULL_HANDLE)
			vkFreeCommandBuffers(m_appData.device, m_appData.dynamicCommandPool, 1, &updateModelMatrixCommandBuffers[imageIndex]);
		
		updateModelMatrixCommandBuffers[imageIndex] = *mbUpdateModelMatrixCommandBuffer;
		
		commandBuffers.push_back(updateModelMatrixCommandBuffers[imageIndex]);
		
		--m_numModelMatrixBuffersToUpdate;
	}
	
	waitSemaphores.push_back(m_appData.imageAvailableSemaphore);
	waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	commandBuffers.push_back(m_appData.pushConstantsCommandBuffers[imageIndex]);
	commandBuffers.push_back(m_appData.currRenderingStyle == app::AppData::RenderingStyle::Wireframe ? m_appData.wireframeCommandBuffers[imageIndex] : m_appData.solidCommandBuffers[imageIndex]);
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
		
		m_numProjMatrixBuffersToUpdate = m_appData.numConcurrentResources;
	}
	else if(res != VK_SUCCESS)
		throw std::runtime_error("failed to present");
}