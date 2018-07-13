#pragma once

#include "TeapotData.h"

#include "vulkan/vulkan.h"

struct GLFWwindow;

namespace app
{

struct AppData
{
	GLFWwindow* window{nullptr};
	bool framebufferResized{false};
	
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
	VkDevice device{VK_NULL_HANDLE};
	
	VkShaderModule vertexShaderModule{VK_NULL_HANDLE};
	VkShaderModule tessControlShaderModule{VK_NULL_HANDLE};
	VkShaderModule tessEvaluationShaderModule{VK_NULL_HANDLE};
	VkShaderModule fragmentShaderModule{VK_NULL_HANDLE};
};

} // namespace app