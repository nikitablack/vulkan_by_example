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