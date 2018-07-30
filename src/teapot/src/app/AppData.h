#pragma once

#include "TeapotData.h"

#include "vulkan/vulkan.h"

struct GLFWwindow;

namespace app
{

struct AppData
{
	enum class RenderingStyle
	{
		Wireframe,
		Solid
	};
	
	GLFWwindow* window{nullptr};
	bool framebufferResized{false};
	float tessLevel{1.0f};
	
	TeapotData teapotData{};
	
	std::vector<char const *> layers{};
	std::vector<char const *> instanceExtensions{};
	std::vector<char const *> deviceExtensions{};
	
	VkInstance instance{VK_NULL_HANDLE};
	VkSurfaceKHR surface{VK_NULL_HANDLE};
	VkSurfaceCapabilitiesKHR surfaceCapabilities{};
	VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
	uint32_t graphicsFamilyQueueIndex{0};
	uint32_t presentFamilyQueueIndex{0};
	VkSurfaceFormatKHR surfaceFormat{};
	VkPresentModeKHR surfacePresentMode{};
	VkExtent2D surfaceExtent{};
	VkPhysicalDeviceFeatures physicalDeviceFeatures{};
	VkPhysicalDeviceProperties physicalDeviceProperties{};
	VkFormat depthFormat{VK_FORMAT_D32_SFLOAT};
	VkDevice device{VK_NULL_HANDLE};
	
	VkShaderModule vertexShaderModule{VK_NULL_HANDLE};
	VkShaderModule tessControlShaderModule{VK_NULL_HANDLE};
	VkShaderModule tessEvaluationShaderModule{VK_NULL_HANDLE};
	VkShaderModule fragmentShaderModule{VK_NULL_HANDLE};
	
	VkDebugUtilsMessengerEXT debugUtilsMessenger{VK_NULL_HANDLE};
	PFN_vkDebugUtilsMessengerCallbackEXT debugCallback{nullptr};
	
	VkRenderPass renderPass{VK_NULL_HANDLE};
	VkDescriptorSetLayout descriptorSetLayout{VK_NULL_HANDLE};
	VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
	VkPipeline wireframePipeline{VK_NULL_HANDLE};
	VkPipeline solidPipeline{VK_NULL_HANDLE};
	
	VkSwapchainKHR swapChain{VK_NULL_HANDLE};
	std::vector<VkFramebuffer> swapChainFramebuffers{};
	std::vector<VkImageView> swapChainImageViews{};
	uint32_t numConcurrentResources{};
	
	VkQueue graphicsQueue{VK_NULL_HANDLE};
	VkQueue presentQueue{VK_NULL_HANDLE};
	
	VkDeviceMemory matricesDeviceMemory{VK_NULL_HANDLE};
	VkBuffer projMatrixBuffer{};
	VkBuffer viewMatrixBuffer{};
	VkBuffer modelMatrixBuffer{};
	VkDeviceSize matrixBufferOffset{};
	
	VkDeviceMemory positionsBufferDeviceMemory{VK_NULL_HANDLE};
	VkBuffer positionsBuffer{VK_NULL_HANDLE};
	
	VkDeviceMemory indexBufferDeviceMemory{VK_NULL_HANDLE};
	VkBuffer indexBuffer{VK_NULL_HANDLE};
	
	VkDeviceMemory patchesBufferDeviceMemory{VK_NULL_HANDLE};
	VkBuffer patchesBuffer{VK_NULL_HANDLE};
	
	VkCommandPool staticCommandPool{VK_NULL_HANDLE};
	VkCommandPool dynamicCommandPool{VK_NULL_HANDLE};
	
	std::vector<VkCommandBuffer> wireframeCommandBuffers{};
	std::vector<VkCommandBuffer> solidCommandBuffers{};
	std::vector<VkCommandBuffer> pushConstantsCommandBuffers{};
	
	VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
	VkDescriptorSet descriptorSet{};
	
	VkSemaphore imageAvailableSemaphore{VK_NULL_HANDLE};
	VkSemaphore presentFinishedSemaphore{VK_NULL_HANDLE};
	
	std::vector<VkFence> commandBufferFences{};
	
	RenderingStyle currRenderingStyle{RenderingStyle::Wireframe};
	
	VkImage depthImage{VK_NULL_HANDLE};
	VkImageView depthImageView{VK_NULL_HANDLE};
	VkDeviceMemory depthImageMemory{VK_NULL_HANDLE};
};

} // namespace app