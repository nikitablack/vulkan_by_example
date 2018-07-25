#include "helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <GLFW/glfw3.h>

namespace app
{

void clear(AppData data)
{
	if(data.device)
	{
		vkDestroySemaphore(data.device, data.imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(data.device, data.presentFinishedSemaphore, nullptr);
		for(VkFence const fence : data.commandBufferFences) vkDestroyFence(data.device, fence, nullptr);
		vkDestroyDescriptorPool(data.device, data.descriptorPool, nullptr);
		vkDestroyCommandPool(data.device, data.renderCommandPool, nullptr);
		vkDestroyCommandPool(data.device, data.pushConstantsCommandPool, nullptr);
		vkDestroyCommandPool(data.device, data.updateMatricesCommandPool, nullptr);
		vkDestroyBuffer(data.device, data.patchesBuffer, nullptr);
		vkFreeMemory(data.device, data.patchesBufferDeviceMemory, nullptr);
		vkDestroyBuffer(data.device, data.indexBuffer, nullptr);
		vkFreeMemory(data.device, data.indexBufferDeviceMemory, nullptr);
		vkDestroyBuffer(data.device, data.positionsBuffer, nullptr);
		vkFreeMemory(data.device, data.positionsBufferDeviceMemory, nullptr);
		vkDestroyBuffer(data.device, data.projMatrixBuffer, nullptr);
		vkDestroyBuffer(data.device, data.viewMatrixBuffer, nullptr);
		vkDestroyBuffer(data.device, data.modelMatrixBuffer, nullptr);
		vkFreeMemory(data.device, data.matricesDeviceMemory, nullptr);
		data = destroy_frame_buffers(data);
		helpers::destroy_swap_chain(data.instance, data.device, data.swapChain);
		vkDestroyPipeline(data.device, data.wireframePipeline, nullptr);
		vkDestroyPipeline(data.device, data.solidPipeline, nullptr);
		vkDestroyPipelineLayout(data.device, data.pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(data.device, data.descriptorSetLayout, nullptr);
		vkDestroyRenderPass(data.device, data.renderPass, nullptr);
		vkDestroyShaderModule(data.device, data.vertexShaderModule, nullptr);
		vkDestroyShaderModule(data.device, data.tessControlShaderModule, nullptr);
		vkDestroyShaderModule(data.device, data.tessEvaluationShaderModule, nullptr);
		vkDestroyShaderModule(data.device, data.fragmentShaderModule, nullptr);
	}
	
	vkDestroyDevice(data.device, nullptr);
	if(data.instance) vkDestroySurfaceKHR(data.instance, data.surface, nullptr);
	if(data.instance) helpers::destroy_debug_utils_messenger(data.instance, data.debugUtilsMessenger);
	vkDestroyInstance(data.instance, nullptr);
	glfwTerminate();
}

} // namespace std