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
using MaybeExtensionProperties = tl::expected<std::vector<VkExtensionProperties>, std::string>;
using MaybeDevice = tl::expected<VkDevice, std::string>;
using MaybeShaderModule = tl::expected<VkShaderModule, std::string>;
using MaybeDebugUtilsMessenger = tl::expected<VkDebugUtilsMessengerEXT, std::string>;
using MaybeRenderPass = tl::expected<VkRenderPass, std::string>;
using MaybeDescriptorSetLayout = tl::expected<VkDescriptorSetLayout, std::string>;
using MaybePipelineLayout = tl::expected<VkPipelineLayout, std::string>;
using MaybePipeline = tl::expected<VkPipeline, std::string>;
using MaybeSwapchain = tl::expected<VkSwapchainKHR, std::string>;
using MaybeImageView = tl::expected<VkImageView, std::string>;
using MaybeFrameBuffer = tl::expected<VkFramebuffer, std::string>;
using MaybeBuffer = tl::expected<VkBuffer, std::string>;
using MaybeMemoryPropertyIndex = tl::expected<uint32_t, std::string>;
using MaybeDeviceMemory = tl::expected<VkDeviceMemory, std::string>;
using MaybeCommandPool = tl::expected<VkCommandPool, std::string>;
using MaybeCommandBuffers = tl::expected<std::vector<VkCommandBuffer>, std::string>;

MaybeInstance create_instance(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
MaybePhysicalDevices get_physical_devices(VkInstance instance);
MaybePhysicalDevicesSurfaceFormats get_physical_devices_surface_formats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
MaybePhysicalDevicesSurfacePresentModes get_physical_device_surface_present_modes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
MaybeExtensionProperties get_physical_device_device_extension_properties(VkPhysicalDevice physicalDevice);
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
MaybeSwapchain create_swap_chain(VkDevice device, VkSurfaceKHR surface, uint32_t imageCount, VkFormat imageFormat, VkColorSpaceKHR imageColorSpace, VkExtent2D imageExtent, VkSurfaceTransformFlagBitsKHR preTransform, VkPresentModeKHR presentMode, VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE, VkSharingMode imageSharingMode = VK_SHARING_MODE_EXCLUSIVE, std::vector<uint32_t> const * queueFamilyIndices = nullptr, VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, uint32_t imageArrayLayers = 1, VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VkBool32 clipped = VK_TRUE);
bool destroy_swap_chain(VkInstance instance, VkDevice device, VkSwapchainKHR swapChain);
MaybeImageView create_image_view(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageSubresourceRange subresourceRange, VkComponentMapping components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY});
MaybeFrameBuffer create_framebuffer(VkDevice device, VkRenderPass renderPass, uint32_t width, uint32_t height, std::vector<VkImageView > const * attachments = nullptr, uint32_t layers = 1);
MaybeBuffer create_buffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, std::vector<uint32_t> const * queueFamilyIndices = nullptr, VkBufferCreateFlags flags = 0, void const * next = nullptr);
MaybeMemoryPropertyIndex get_supported_memory_property_index(VkPhysicalDevice physicalDevice, uint32_t supportedMemoryTypeBits, VkMemoryPropertyFlags desiredMemoryFlags);
MaybeDeviceMemory allocate_device_memory(VkDevice device, VkDeviceSize allocationSize, uint32_t memoryTypeIndex, void const * next = nullptr);
MaybeCommandPool create_command_pool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
MaybeCommandBuffers allocate_command_buffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

} // namespace app::helpers