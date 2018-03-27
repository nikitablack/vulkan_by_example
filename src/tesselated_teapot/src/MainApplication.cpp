#include "app/App.h"
#include "app/create_swap_chain.h"
#include "vk_helpers/VkStructHelpers.h"
#include "MainApplication.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <array>
#include <cassert>
#include <stdexcept>

using namespace app;
using namespace details;
using namespace std;
using namespace vk_helpers;

namespace
{
	void onKeyPress(GLFWwindow * const window, int const key, int const /*scancode*/, int const action, int const /*mods*/)
	{
		AppData * const appData{static_cast<AppData *>(glfwGetWindowUserPointer(window))};
		
		if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if(appData->tessLevel < appData->physicalDeviceProperties.limits.maxTessellationGenerationLevel)
				appData->tessLevel += 0.1f;
		}
		else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if(appData->tessLevel > 1)
				appData->tessLevel -= 0.1;
		}
		else if (key == GLFW_KEY_SPACE && (action == GLFW_RELEASE))
		{
			appData->currCommandBuffer = (appData->currCommandBuffer == &appData->commandBuffersWireframe) ? &appData->commandBuffersSolid : &appData->commandBuffersWireframe;
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
	_appData.tessLevel = 1.0f;
	
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
	                          .and_then(app::create_frame_buffers)
	                          .and_then(create_matrices_buffers)
	                          .map(get_device_qeues)
	                          .and_then(create_positions_buffer)
	                          .and_then(create_index_buffer)
	                          .and_then(create_patches_buffer)
	                          .and_then(create_command_pools)
	                          .and_then(allocate_command_buffers)
	                          .and_then(create_present_semaphores)
	                          .and_then(create_fences)
	                          .and_then(create_descriptor_pool)
	                          .and_then(allocate_descriptor_sets)
	                          .map(update_descriptor_set)
	                          .and_then(record_rendering_command_buffers)};
	
	if (!maybeAppData)
		throw runtime_error{maybeAppData.error()};
	
	_appData = *maybeAppData;
	
	_appData.currCommandBuffer = &_appData.commandBuffersWireframe;
	
	_numProjMatrixBuffersToUpdate = static_cast<uint32_t>(_appData.swapChainFramebuffers.size());
	_numViewMatrixBuffersToUpdate = static_cast<uint32_t>(_appData.swapChainFramebuffers.size());
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
	
	if(vkDeviceWaitIdle(_appData.device) != VK_SUCCESS)
		throw runtime_error{ "failed to wait device idle" };
}

void MainApplication::render()
{
	assert(_appData.currCommandBuffer);
	
	uint32_t imageIndex{};
	if(vkAcquireNextImageKHR(_appData.device, _appData.swapChain, numeric_limits<uint64_t>::max(), _appData.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
		throw runtime_error("failed to acquire swap chain image");
	
	array<VkFence, 1> waitingFences{_appData.commandBufferFences[imageIndex]};
	
	if(vkWaitForFences(_appData.device, static_cast<uint32_t>(waitingFences.size()), waitingFences.data(), VK_TRUE, numeric_limits<uint64_t>::max()) != VK_SUCCESS)
		throw runtime_error("failed to wait for the fence");
	
	if(vkResetFences(_appData.device, static_cast<uint32_t>(waitingFences.size()), waitingFences.data()) != VK_SUCCESS)
		throw runtime_error("failed to reset fence");
	
	{
		VkCommandBufferBeginInfo const beginInfo{get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
		
		if(vkBeginCommandBuffer(_appData.commandBuffersPushConstants[imageIndex], &beginInfo) != VK_SUCCESS)
			throw runtime_error("failed to begin command buffer");
		
		vkCmdPushConstants(_appData.commandBuffersPushConstants[imageIndex], _appData.pipelineLayout, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, 0, sizeof(_appData.tessLevel), &_appData.tessLevel);
		
		if(vkEndCommandBuffer(_appData.commandBuffersPushConstants[imageIndex]) != VK_SUCCESS)
			throw runtime_error("failed to end command buffer");
	}
	
	static vector<VkSemaphore> waitSemaphores{};
	static vector<VkPipelineStageFlags> waitStages{};
	static vector<VkCommandBuffer> commandBuffers{};
	static vector<VkSemaphore> signalSemaphores{};
	
	waitSemaphores.clear();
	waitStages.clear();
	commandBuffers.clear();
	signalSemaphores.clear();
	
	if(_numProjMatrixBuffersToUpdate > 0)
	{
		MaybeCommandBuffer const mbCommandBuffer{update_projection_matrix(_appData, imageIndex)};
		
		if(!mbCommandBuffer)
			throw runtime_error(mbCommandBuffer.error());
		
		commandBuffers.push_back(*mbCommandBuffer);
		
		--_numProjMatrixBuffersToUpdate;
	}
	
	if(_numViewMatrixBuffersToUpdate > 0)
	{
		MaybeCommandBuffer const mbCommandBuffer{update_view_matrix(_appData, imageIndex)};
		
		if(!mbCommandBuffer)
			throw runtime_error(mbCommandBuffer.error());
		
		commandBuffers.push_back(*mbCommandBuffer);
		
		--_numViewMatrixBuffersToUpdate;
	}
	
	{
		MaybeCommandBuffer const mbCommandBuffer{update_model_matrix(_appData, imageIndex)};
		
		if (!mbCommandBuffer)
			throw runtime_error(mbCommandBuffer.error());
		
		commandBuffers.push_back(*mbCommandBuffer);
	}
	
	waitSemaphores.push_back(_appData.imageAvailableSemaphore);
	waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	commandBuffers.push_back(_appData.commandBuffersPushConstants[imageIndex]);
	commandBuffers.push_back((*_appData.currCommandBuffer)[imageIndex]);
	signalSemaphores.push_back(_appData.renderFinishedSemaphore);
	
	VkSubmitInfo const submitInfo{get_submit_info(&commandBuffers, &waitSemaphores, &waitStages, &signalSemaphores)};
	
	if(vkQueueSubmit(_appData.graphicsQueue, 1, &submitInfo, _appData.commandBufferFences[imageIndex]) != VK_SUCCESS)
	{
		throw runtime_error("failed to submit draw command buffer");
	}
	
	static vector<VkSwapchainKHR> swapChains{};
	swapChains.clear();
	swapChains.push_back(_appData.swapChain);
	
	static vector<uint32_t> imageIndices{};
	imageIndices.clear();
	imageIndices.push_back(imageIndex);
	
	VkPresentInfoKHR presentInfo{get_present_info_KHR(&swapChains, &imageIndices, &signalSemaphores)};
	
	if(vkQueuePresentKHR(_appData.presentQueue, &presentInfo) != VK_SUCCESS)
	{
		throw runtime_error("failed to present");
	}
}