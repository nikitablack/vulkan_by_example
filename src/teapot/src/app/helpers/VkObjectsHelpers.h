#pragma once

#include "tl/expected.hpp"
#include "vulkan/vulkan.h"

#include <string>
#include <vector>

namespace app::helpers
{

using MaybeInstance = tl::expected<VkInstance, std::string>;
using MaybePhysicalDevices = tl::expected<std::vector<VkPhysicalDevice>, std::string>;
using MaybePhysicalDevicesSurfaceFormats = tl::expected<std::vector<VkSurfaceFormatKHR>, std::string>;
using MaybePhysicalDevicesSurfacePresentModes = tl::expected<std::vector<VkPresentModeKHR>, std::string>;
using MaybeDevice = tl::expected<VkDevice, std::string>;
using MaybeShaderModule = tl::expected<VkShaderModule, std::string>;
using MaybeDebugUtilsMessenger = tl::expected<VkDebugUtilsMessengerEXT, std::string>;
using MaybeRenderPass = tl::expected<VkRenderPass, std::string>;
using MaybeDescriptorSetLayout = tl::expected<VkDescriptorSetLayout, std::string>;
using MaybePipelineLayout = tl::expected<VkPipelineLayout, std::string>;
using MaybePipeline = tl::expected<VkPipeline, std::string>;

MaybeInstance create_instance(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
MaybePhysicalDevices get_physical_devices(VkInstance instance);
MaybePhysicalDevicesSurfaceFormats get_physical_devices_surface_formats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
MaybePhysicalDevicesSurfacePresentModes get_physical_device_surface_present_modes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
std::vector<VkQueueFamilyProperties> get_queue_family_properties(VkPhysicalDevice physicalDevice);
MaybeDevice create_device(VkPhysicalDevice physicalDevice, std::vector<uint32_t> const * queueIndices, std::vector<std::vector<float>> const * queuePriorities, VkPhysicalDeviceFeatures const * features = nullptr, std::vector<char const *> const * extensions = nullptr);
MaybeShaderModule create_shader_module(VkDevice device, std::vector<char> const * shaderCode);
MaybeDebugUtilsMessenger create_debug_utils_messenger(VkInstance instance, VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback, void * pUserData = nullptr);
bool destroy_debug_utils_messenger(VkInstance instance, VkDebugUtilsMessengerEXT messenger);
bool set_debug_utils_object_name(VkInstance instance, VkDevice device, VkObjectType objectType, uint64_t objectHandle, char const * pObjectName);
MaybeRenderPass create_render_pass(VkDevice device, std::vector<VkSubpassDescription> const * subpasses, std::vector<VkAttachmentDescription> const * attachments = nullptr, std::vector<VkSubpassDependency> const * dependencies = nullptr);
MaybeDescriptorSetLayout create_descriptor_set_layout(VkDevice device, std::vector<VkDescriptorSetLayoutBinding> const * bindings = nullptr);
MaybePipelineLayout create_pipeline_layout(VkDevice device, std::vector<VkDescriptorSetLayout> const * setLayouts = nullptr, std::vector<VkPushConstantRange> const * pushConstantRanges = nullptr);
MaybePipeline create_pipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, uint32_t subpass, VkPipelineCreateFlags flags, std::vector<VkPipelineShaderStageCreateInfo> const * shaderStages, VkPipelineVertexInputStateCreateInfo const * vertexInputState, VkPipelineInputAssemblyStateCreateInfo const * inputAssemblyState, VkPipelineRasterizationStateCreateInfo const * rasterizationState, VkPipelineColorBlendStateCreateInfo const * colorBlendState = nullptr, VkPipelineDepthStencilStateCreateInfo const * depthStencilState = nullptr, VkPipelineViewportStateCreateInfo const * viewportState = nullptr, VkPipelineMultisampleStateCreateInfo const * multisampleState = nullptr, VkPipelineTessellationStateCreateInfo const * tesselationState = nullptr, VkPipelineDynamicStateCreateInfo const * dynamicState = nullptr, VkPipeline basePipelineHandle = VK_NULL_HANDLE, VkPipelineCache pipelineCache = VK_NULL_HANDLE);

} // namespace app::helpers