#include "app/App.h"
#include "vk_helpers/VkObjectsHelper.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>

using namespace std;
using namespace vk_helpers;

namespace app
{
	void clear(AppData appData)
	{
		if(appData.device)
		{
			vkDestroyDescriptorPool(appData.device, appData.descriptorPool, nullptr);
			for_each(appData.commandBufferFences.begin(), appData.commandBufferFences.end(), [&appData](VkFence fence){vkDestroyFence(appData.device, fence, nullptr);});
			vkDestroySemaphore(appData.device, appData.renderFinishedSemaphore, nullptr);
			vkDestroySemaphore(appData.device, appData.imageAvailableSemaphore, nullptr);
			vkDestroyCommandPool(appData.device, appData.commandPoolRender, nullptr);
			vkDestroyCommandPool(appData.device, appData.commandPoolPushConstants, nullptr);
			vkDestroyCommandPool(appData.device, appData.commandPoolMatrices, nullptr);
			vkDestroyBuffer(appData.device, appData.patchesBuffer, nullptr);
			vkFreeMemory(appData.device, appData.patchesBufferDeviceMemory, nullptr);
			vkDestroyBuffer(appData.device, appData.indexBuffer, nullptr);
			vkFreeMemory(appData.device, appData.indexBufferDeviceMemory, nullptr);
			vkDestroyBuffer(appData.device, appData.positionsBuffer, nullptr);
			vkFreeMemory(appData.device, appData.positionsBufferDeviceMemory, nullptr);
			for_each(appData.modelMatricesBuffers.begin(), appData.modelMatricesBuffers.end(), [&appData](VkBuffer buffer){vkDestroyBuffer(appData.device, buffer, nullptr);});
			for_each(appData.viewMatricesBuffers.begin(), appData.viewMatricesBuffers.end(), [&appData](VkBuffer buffer){vkDestroyBuffer(appData.device, buffer, nullptr);});
			for_each(appData.projMatricesBuffers.begin(), appData.projMatricesBuffers.end(), [&appData](VkBuffer buffer){vkDestroyBuffer(appData.device, buffer, nullptr);});
			vkFreeMemory(appData.device, appData.matricesDeviceMemory, nullptr);
			for_each(appData.swapChainImageViews.begin(), appData.swapChainImageViews.end(), [&appData](VkImageView imageView){vkDestroyImageView(appData.device, imageView, nullptr);});
			for_each(appData.swapChainFramebuffers.begin(), appData.swapChainFramebuffers.end(), [&appData](VkFramebuffer framebuffer){vkDestroyFramebuffer(appData.device, framebuffer, nullptr);});
			vkDestroySwapchainKHR(appData.device, appData.swapChain, nullptr);
			vkDestroyPipeline(appData.device, appData.graphicsPipelineWireframe, nullptr);
			vkDestroyPipeline(appData.device, appData.graphicsPipelineSolid, nullptr);
			vkDestroyPipelineLayout(appData.device, appData.pipelineLayout, nullptr);
			vkDestroyDescriptorSetLayout(appData.device, appData.descriptorSetLayout, nullptr);
			vkDestroyRenderPass(appData.device, appData.renderPass, nullptr);
			vkDestroyShaderModule(appData.device, appData.vertexShaderModule, nullptr);
			vkDestroyShaderModule(appData.device, appData.tessControlShaderModule, nullptr);
			vkDestroyShaderModule(appData.device, appData.tessEvaluationShaderModule, nullptr);
			vkDestroyShaderModule(appData.device, appData.fragmentShaderModule, nullptr);
		}
		
		vkDestroyDevice(appData.device, nullptr);
		if(appData.instance) vkDestroySurfaceKHR(appData.instance, appData.surface, nullptr);
		destroy_debug_report_callback(appData.instance, appData.debugCallback);
		vkDestroyInstance(appData.instance, nullptr);
		glfwTerminate();
	}
}