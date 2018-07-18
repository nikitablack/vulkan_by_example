#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace app::helpers
{

VkInstanceCreateInfo get_instance_create_info(std::vector<char const *> const * extensions = nullptr, std::vector<char const *> const * layers = nullptr, VkApplicationInfo const * applicationInfo = nullptr);
VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t queueFamilyIndex, std::vector<float> const * queuePriorities);
VkDeviceCreateInfo get_device_create_info(std::vector<VkDeviceQueueCreateInfo> const * queueCreateInfos, VkPhysicalDeviceFeatures const * deviceFeatures = nullptr, std::vector<char const *> const * deviceExtensions = nullptr);
VkShaderModuleCreateInfo get_shader_module_create_info(std::vector<char> const * shaderCode);
VkDebugUtilsMessengerCreateInfoEXT get_debug_utils_messenger_create_info(VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback, void * pUserData = nullptr);
VkDebugUtilsObjectNameInfoEXT get_debug_utils_object_name_info(VkObjectType objectType, uint64_t objectHandle, char const * pObjectName);
VkRenderPassCreateInfo get_render_pass_create_info(std::vector<VkSubpassDescription> const * subpasses, std::vector<VkAttachmentDescription> const * attachments = nullptr, std::vector<VkSubpassDependency> const * dependencies = nullptr);
VkAttachmentDescription get_attachment_description(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentDescriptionFlags flags = 0, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE, VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE);
VkAttachmentReference get_attachment_reference(uint32_t attachment, VkImageLayout layout);
VkSubpassDescription get_subpass_description(std::vector<VkAttachmentReference> const * colorAttachments = nullptr, VkAttachmentReference const * depthStencilAttachment = nullptr, std::vector<VkAttachmentReference> const * inputAttachments = nullptr, std::vector<VkAttachmentReference> const * resolveAttachments = nullptr, std::vector<uint32_t> const * preserveAttachments = nullptr);
VkDescriptorSetLayoutCreateInfo get_descriptor_set_layout_create_info(std::vector<VkDescriptorSetLayoutBinding> const * bindings = nullptr);
VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType descriptorType, uint32_t binding, VkShaderStageFlags stageFlags, uint32_t descriptorCount, std::vector<VkSampler> const * immutableSamplers = nullptr);
VkPipelineLayoutCreateInfo get_pipeline_layout_create_info(std::vector<VkDescriptorSetLayout> const * setLayouts = nullptr, std::vector<VkPushConstantRange> const * pushConstantRanges = nullptr);
VkGraphicsPipelineCreateInfo get_graphics_pipeline_create_info(VkRenderPass renderPass, VkPipelineLayout layout, uint32_t subpass, VkPipelineCreateFlags flags, std::vector<VkPipelineShaderStageCreateInfo> const * shaderStages, VkPipelineVertexInputStateCreateInfo const *  vertexInputState, VkPipelineInputAssemblyStateCreateInfo const * inputAssemblyState, VkPipelineRasterizationStateCreateInfo const * rasterizationState, VkPipelineColorBlendStateCreateInfo const * colorBlendState = nullptr, VkPipelineDepthStencilStateCreateInfo const * depthStencilState = nullptr, VkPipelineViewportStateCreateInfo const * viewportState = nullptr, VkPipelineMultisampleStateCreateInfo const * multisampleState = nullptr, VkPipelineTessellationStateCreateInfo const * tesselationState = nullptr, VkPipelineDynamicStateCreateInfo const * dynamicState = nullptr, VkPipeline basePipelineHandle = VK_NULL_HANDLE, int32_t basePipelineIndex = -1);
VkPushConstantRange get_push_constant_range(VkShaderStageFlags stageFlags, uint32_t size, uint32_t offset = 0);
VkPipelineShaderStageCreateInfo get_pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule module, char const * name = nullptr, VkSpecializationInfo const * specializationInfo = nullptr);
VkVertexInputBindingDescription get_vertex_input_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
VkVertexInputAttributeDescription get_vertex_input_attribute_description(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);
VkPipelineVertexInputStateCreateInfo get_pipeline_vertex_input_state_create_info(std::vector<VkVertexInputBindingDescription> const * vertexInputBindings = nullptr, std::vector<VkVertexInputAttributeDescription> const * vertexInputAttributes = nullptr);
VkPipelineInputAssemblyStateCreateInfo get_pipeline_input_assembly_state_create_info(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VkBool32 primitiveRestartEnable = VK_FALSE);
VkPipelineRasterizationStateCreateInfo get_pipeline_rasterization_state_create_info(VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT, VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE, VkBool32 depthClampEnable = VK_FALSE, VkBool32 rasterizerDiscardEnable = VK_FALSE, float lineWidth = 1.0f, VkBool32 depthBiasEnable = VK_FALSE, float depthBiasConstantFactor = 0.0f, float depthBiasClamp = 0.0f, float depthBiasSlopeFactor = 0.0f);
VkPipelineDynamicStateCreateInfo get_pipeline_dynamic_state_create_info(std::vector<VkDynamicState> const * dynamicStates);
VkPipelineViewportStateCreateInfo get_pipeline_viewport_state_create_info(std::vector<VkViewport> const * viewports, std::vector<VkRect2D> const * scissors);
VkPipelineMultisampleStateCreateInfo get_pipeline_multisample_state_create_info(VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, VkBool32 sampleShadingEnable = VK_FALSE, float minSampleShading = 1.0f, VkSampleMask const * sampleMask = nullptr, VkBool32 alphaToCoverageEnable = VK_FALSE, VkBool32 alphaToOneEnable = VK_FALSE);
VkPipelineTessellationStateCreateInfo get_pipeline_tessellation_state_create_info(uint32_t patchControlPoints);
VkPipelineColorBlendAttachmentState get_pipeline_color_blend_attachment_state(VkBool32 blendEnable = VK_FALSE, VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ONE, VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, VkBlendOp colorBlendOp = VK_BLEND_OP_ADD, VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD, VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
VkPipelineColorBlendStateCreateInfo get_pipeline_color_blend_state_create_info(std::vector<VkPipelineColorBlendAttachmentState> const * attachments = nullptr, VkBool32 logicOpEnable = VK_FALSE, VkLogicOp logicOp = VK_LOGIC_OP_CLEAR, std::vector<float> blendConstants = {0.0f, 0.0f, 0.0f, 0.0f});

} // namespace app::helpers