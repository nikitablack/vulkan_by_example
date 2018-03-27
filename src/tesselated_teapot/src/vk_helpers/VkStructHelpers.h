#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace vk_helpers
{
	VkInstanceCreateInfo get_instance_create_info(std::vector<char const *> const * extensions = nullptr,
	                                              std::vector<char const *> const * layers = nullptr);
	VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t queueFamilyIndex, std::vector<float> const * queuePriorities);
	VkDeviceCreateInfo get_device_create_info(std::vector<VkDeviceQueueCreateInfo> const * queueCreateInfos,
	                                          std::vector<char const *> const * deviceExtensions,
	                                          VkPhysicalDeviceFeatures const * deviceFeatures);
	VkShaderModuleCreateInfo get_shader_module_create_info(std::vector<char> const * shaderCode);
	VkDebugReportCallbackCreateInfoEXT get_debug_report_callback_createInfo(PFN_vkDebugReportCallbackEXT callback);
	VkAttachmentDescription get_attachment_description(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout);
	VkAttachmentReference get_attachment_reference(uint32_t attachment, VkImageLayout layout);
	VkSubpassDescription get_subpass_description(std::vector<VkAttachmentReference> const * colorAttachments = nullptr,
	                                             VkAttachmentReference const * depthStencilAttachment = nullptr);
	VkRenderPassCreateInfo get_render_pass_create_info(std::vector<VkSubpassDescription> const * subpasses,
	                                                   std::vector<VkAttachmentDescription> const * attachments = nullptr,
	                                                   std::vector<VkSubpassDependency> const * dependencies = nullptr);
	VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType descriptorType,
	                                                               uint32_t binding,
	                                                               VkShaderStageFlags stageFlags,
	                                                               uint32_t descriptorCount = 1);
	VkDescriptorSetLayoutCreateInfo get_descriptor_set_layout_create_info(std::vector<VkDescriptorSetLayoutBinding> const * bindings = nullptr);
	VkPushConstantRange get_push_constant_range(VkShaderStageFlags stageFlags, uint32_t size, uint32_t offset = 0);
	VkPipelineLayoutCreateInfo get_pipeline_layout_create_info(std::vector<VkDescriptorSetLayout> const * setLayouts = nullptr,
	                                                           std::vector<VkPushConstantRange> const * pushConstantRanges = nullptr);
	VkPipelineShaderStageCreateInfo get_pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule module);
	VkVertexInputBindingDescription get_vertex_input_binding_description(uint32_t binding, uint32_t stride);
	VkVertexInputAttributeDescription get_vertex_input_attribute_description(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset = 0);
	VkPipelineVertexInputStateCreateInfo get_pipeline_vertex_input_state_create_info(std::vector<VkVertexInputBindingDescription> const * vertexInputBindings = nullptr,
	                                                                                 std::vector<VkVertexInputAttributeDescription> const  * vertexInputAttributes = nullptr);
	VkPipelineInputAssemblyStateCreateInfo get_pipeline_input_assembly_state_create_info(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	VkPipelineRasterizationStateCreateInfo get_pipeline_rasterization_state_create_info(VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL,
	                                                                                    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT);
	VkViewport get_viewport(float width, float height);
	VkRect2D get_scissor(VkExtent2D extent);
	VkPipelineViewportStateCreateInfo get_pipeline_viewport_state_create_info(std::vector<VkViewport> const * viewports,
	                                                                          std::vector<VkRect2D> const * scissors);
	VkPipelineMultisampleStateCreateInfo get_pipeline_multisample_state_create_info();
	VkPipelineTessellationStateCreateInfo get_pipeline_tessellation_state_create_info(uint32_t patchControlPoints);
	VkPipelineColorBlendAttachmentState get_pipeline_color_blend_attachment_state();
	VkPipelineColorBlendStateCreateInfo get_pipeline_color_blend_state_create_info(std::vector<VkPipelineColorBlendAttachmentState> const * colorBlendAttachments = nullptr);
	VkGraphicsPipelineCreateInfo get_graphics_pipeline_create_info(VkRenderPass renderPass,
	                                                               VkPipelineLayout pipelineLayout,
	                                                               std::vector<VkPipelineShaderStageCreateInfo> const * shaderStages,
	                                                               VkPipelineVertexInputStateCreateInfo const *  vertexInputState,
	                                                               VkPipelineInputAssemblyStateCreateInfo const * inputAssemblyState,
	                                                               VkPipelineRasterizationStateCreateInfo const * rasterizationState,
	                                                               VkPipelineViewportStateCreateInfo const * viewportState = nullptr,
	                                                               VkPipelineMultisampleStateCreateInfo const * multisampleState = nullptr,
	                                                               VkPipelineTessellationStateCreateInfo const * tesselationState = nullptr,
	                                                               VkPipelineColorBlendStateCreateInfo const * colorBlendState = nullptr,
	                                                               VkPipelineDepthStencilStateCreateInfo const * depthStencilState = nullptr,
	                                                               VkPipelineCreateFlags flags = 0,
	                                                               VkPipeline basePipelineHandle = VK_NULL_HANDLE);
    VkSwapchainCreateInfoKHR get_swap_chain_create_info(VkSurfaceKHR surface,
                                                        uint32_t imageCount,
                                                        VkFormat imageFormat,
                                                        VkColorSpaceKHR
                                                        imageColorSpace,
                                                        VkExtent2D imageExtent,
                                                        VkSurfaceTransformFlagBitsKHR preTransform,
                                                        VkPresentModeKHR presentMode);
    VkImageViewCreateInfo get_image_view_create_info(VkImage image, VkFormat format, VkImageAspectFlags imageAspectFlags);
    VkFramebufferCreateInfo get_frame_buffer_create_info(VkRenderPass renderPass,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         std::vector<VkImageView > const * attachments = nullptr);
    VkBufferCreateInfo get_buffer_create_info(VkDeviceSize size, VkBufferUsageFlags usage);
    VkMemoryAllocateInfo get_memory_allocate_info(VkDeviceSize allocationSize, uint32_t memoryTypeIndex);
    VkMappedMemoryRange get_mapped_memory_range(VkDeviceMemory memory, VkDeviceSize size, VkDeviceSize offset = 0);
    VkCommandPoolCreateInfo get_command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
    VkCommandBufferAllocateInfo get_command_buffer_allocate_info(VkCommandPool commandPool, uint32_t commandBufferCount = 1);
    VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);
    VkBufferMemoryBarrier get_buffer_memory_barrier(VkBuffer buffer,
                                                    VkAccessFlags srcAccessMask,
                                                    VkAccessFlags dstAccessMask,
                                                    VkDeviceSize size,
                                                    VkDeviceSize offset = 0,
                                                    uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                                    uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
    VkBufferCopy get_buffer_copy(VkDeviceSize size);
    VkSubmitInfo get_submit_info(std::vector<VkCommandBuffer> const * commandBuffers,
                                 std::vector<VkSemaphore> const * waitSemaphores = nullptr,
                                 std::vector<VkPipelineStageFlags> const * waitStages = nullptr,
                                 std::vector<VkSemaphore> const * signalSemaphores = nullptr);
    VkSemaphoreCreateInfo get_semaphore_create_info();
    VkFenceCreateInfo get_fence_create_info(VkFenceCreateFlags flags = 0);
    VkDescriptorPoolSize get_descriptor_pool_size(VkDescriptorType type, uint32_t descriptorCount);
    VkDescriptorPoolCreateInfo get_descriptor_pool_create_info(uint32_t maxSets, std::vector<VkDescriptorPoolSize> const * pollSizes);
    VkDescriptorSetAllocateInfo get_descriptor_set_allocate_info(VkDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> const * const layouts);
    VkDescriptorBufferInfo get_descriptor_buffer_info(VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset = 0);
    VkWriteDescriptorSet get_write_descriptor_set(VkDescriptorSet dstSet,
                                                  uint32_t dstBinding,
                                                  VkDescriptorType descriptorType,
                                                  std::vector<VkDescriptorBufferInfo> const * bufferInfos);
    VkRenderPassBeginInfo get_render_pass_begin_info(VkRenderPass renderPass,
                                                     VkFramebuffer framebuffer,
                                                     VkExtent2D renderExtent,
                                                     std::vector<VkClearValue> const * clearValues = nullptr);
    VkPresentInfoKHR get_present_info_KHR(std::vector<VkSwapchainKHR > const * swapChains,
                                          std::vector<uint32_t > const * imageIndices,
                                          std::vector<VkSemaphore> const * signalSemaphores = nullptr);
}