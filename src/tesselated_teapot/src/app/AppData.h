#pragma once

#include "TeapotData.h"

#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace app
{
	struct AppData
	{
		GLFWwindow* window{nullptr};
		
		TeapotData teapotData{};
		float tessLevel{1.0f};
		
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
		VkDevice device{VK_NULL_HANDLE};
		
		VkShaderModule vertexShaderModule{VK_NULL_HANDLE};
		VkShaderModule tessControlShaderModule{VK_NULL_HANDLE};
		VkShaderModule tessEvaluationShaderModule{VK_NULL_HANDLE};
		VkShaderModule fragmentShaderModule{VK_NULL_HANDLE};
		
		PFN_vkDebugReportCallbackEXT debugCallbackPtr{nullptr};
		VkDebugReportCallbackEXT debugCallback{VK_NULL_HANDLE};
		
		VkRenderPass renderPass{VK_NULL_HANDLE};
		VkDescriptorSetLayout descriptorSetLayout{VK_NULL_HANDLE};
		VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
		VkPipeline graphicsPipelineWireframe{VK_NULL_HANDLE};
		VkPipeline graphicsPipelineSolid{VK_NULL_HANDLE};
		
		VkSwapchainKHR swapChain{VK_NULL_HANDLE};
		std::vector<VkFramebuffer> swapChainFramebuffers{};
		std::vector<VkImageView> swapChainImageViews{};
		
		VkDeviceMemory matricesDeviceMemory{VK_NULL_HANDLE};
		std::vector<VkBuffer> projMatricesBuffers{};
		std::vector<VkBuffer> viewMatricesBuffers{};
		std::vector<VkBuffer> modelMatricesBuffers{};
		std::vector<VkDeviceSize> projMatricesOffsets{};
		std::vector<VkDeviceSize> viewMatricesOffsets{};
		std::vector<VkDeviceSize> modelMatricesOffsets{};
		
		VkQueue graphicsQueue{VK_NULL_HANDLE};
		VkQueue presentQueue{VK_NULL_HANDLE};
		
		VkDeviceMemory positionsBufferDeviceMemory{VK_NULL_HANDLE};
		VkBuffer positionsBuffer{VK_NULL_HANDLE};
		
		VkDeviceMemory indexBufferDeviceMemory{VK_NULL_HANDLE};
		VkBuffer indexBuffer{VK_NULL_HANDLE};
		
		VkDeviceMemory patchesBufferDeviceMemory{VK_NULL_HANDLE};
		VkBuffer patchesBuffer{VK_NULL_HANDLE};
	};
}