#include "helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <GLFW/glfw3.h>

namespace app
{

void clear(AppData data)
{
	if(data.device)
	{
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