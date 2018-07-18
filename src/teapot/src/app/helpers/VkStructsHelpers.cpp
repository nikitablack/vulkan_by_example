#include "app/helpers/VkStructsHelpers.h"

#include <cassert>

using namespace std;

namespace app::helpers
{

VkInstanceCreateInfo get_instance_create_info(vector<char const *> const * const extensions, vector<char const *> const * const layers, VkApplicationInfo const * const applicationInfo)
{
	VkInstanceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pApplicationInfo = applicationInfo;
	info.enabledLayerCount = (layers) ? static_cast<uint32_t>(layers->size()) : 0;
	info.ppEnabledLayerNames = (layers) ? layers->data() : nullptr;
	info.enabledExtensionCount = (extensions) ? static_cast<uint32_t>(extensions->size()) : 0;
	info.ppEnabledExtensionNames = (extensions) ? extensions->data() : nullptr;
	
	return info;
}

VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t const queueFamilyIndex, vector<float> const * const queuePriorities)
{
	assert(queuePriorities);
	assert(!queuePriorities->empty());
	
	VkDeviceQueueCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.queueFamilyIndex = queueFamilyIndex;
	info.queueCount = static_cast<uint32_t>(queuePriorities->size());
	info.pQueuePriorities = queuePriorities->data();
	
	return info;
}

VkDeviceCreateInfo get_device_create_info(vector<VkDeviceQueueCreateInfo> const * const queueCreateInfos, VkPhysicalDeviceFeatures const * const deviceFeatures, vector<char const *> const * const deviceExtensions)
{
	assert(queueCreateInfos);
	assert(!queueCreateInfos->empty());
	
	VkDeviceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos->size());
	info.pQueueCreateInfos = queueCreateInfos->data();
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = nullptr;
	info.enabledExtensionCount = deviceExtensions ? static_cast<uint32_t>(deviceExtensions->size()) : 0;
	info.ppEnabledExtensionNames = deviceExtensions ? deviceExtensions->data() : nullptr;
	info.pEnabledFeatures = deviceFeatures;
	
	return info;
}

VkShaderModuleCreateInfo get_shader_module_create_info(vector<char> const * const shaderCode)
{
	assert(shaderCode);
	assert(!shaderCode->empty());
	assert(shaderCode->size() % 4 == 0);
	
	VkShaderModuleCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.codeSize = shaderCode->size();
	info.pCode = reinterpret_cast<uint32_t const *>(shaderCode->data());
	
	return info;
}

VkDebugUtilsMessengerCreateInfoEXT get_debug_utils_messenger_create_info(VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity, VkDebugUtilsMessageTypeFlagsEXT const messageType, PFN_vkDebugUtilsMessengerCallbackEXT const pfnUserCallback, void * pUserData)
{
	assert(messageSeverity);
	assert(messageType);
	
	VkDebugUtilsMessengerCreateInfoEXT info{};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.pNext = nullptr;
	info.flags = 0;
	info.messageSeverity = messageSeverity;
	info.messageType = messageType;
	info.pfnUserCallback = pfnUserCallback;
	info.pUserData = pUserData;
	
	return info;
}

VkDebugUtilsObjectNameInfoEXT get_debug_utils_object_name_info(VkObjectType const objectType, uint64_t const objectHandle, char const * const pObjectName)
{
	assert(objectType != VK_OBJECT_TYPE_UNKNOWN);
	assert(objectHandle);
	
	VkDebugUtilsObjectNameInfoEXT info{};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.pNext = nullptr;
	info.objectType = objectType;
	info.objectHandle = objectHandle;
	info.pObjectName = pObjectName;
	
	return info;
}

VkRenderPassCreateInfo get_render_pass_create_info(vector<VkSubpassDescription> const * const subpasses, vector<VkAttachmentDescription> const * const attachments, vector<VkSubpassDependency> const * const dependencies)
{
	assert(subpasses);
	assert(!subpasses->empty());
	
	VkRenderPassCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.attachmentCount = (attachments != nullptr) ? static_cast<uint32_t>(attachments->size()) : 0;
	info.pAttachments = (attachments != nullptr) ? attachments->data() : nullptr;
	info.subpassCount = static_cast<uint32_t>(subpasses->size());
	info.pSubpasses = subpasses->data();
	info.dependencyCount = (dependencies != nullptr) ? static_cast<uint32_t>(dependencies->size()) : 0;
	info.pDependencies = (dependencies != nullptr) ? dependencies->data() : nullptr;
	
	return info;
}

VkAttachmentDescription get_attachment_description(VkFormat const format, VkImageLayout const initialLayout, VkImageLayout const finalLayout, VkAttachmentDescriptionFlags const flags, VkSampleCountFlagBits const samples, VkAttachmentLoadOp const loadOp, VkAttachmentStoreOp const storeOp, VkAttachmentLoadOp const stencilLoadOp, VkAttachmentStoreOp const stencilStoreOp)
{
	VkAttachmentDescription desc{};
	desc.flags = flags;
	desc.format = format;
	desc.samples = samples;
	desc.loadOp = loadOp;
	desc.storeOp = storeOp;
	desc.stencilLoadOp = stencilLoadOp;
	desc.stencilStoreOp = stencilStoreOp;
	desc.initialLayout = initialLayout;
	desc.finalLayout = finalLayout;
	
	return desc;
}

VkAttachmentReference get_attachment_reference(uint32_t const attachment, VkImageLayout const layout)
{
	VkAttachmentReference ref{};
	ref.attachment = attachment;
	ref.layout = layout;
	
	return ref;
}

VkSubpassDescription get_subpass_description(vector<VkAttachmentReference> const * const colorAttachments, VkAttachmentReference const * const depthStencilAttachment, vector<VkAttachmentReference> const * const inputAttachments, vector<VkAttachmentReference> const * const resolveAttachments, vector<uint32_t> const * const preserveAttachments)
{
	assert(resolveAttachments ? colorAttachments != nullptr : true);
	assert(resolveAttachments ? resolveAttachments->size() == colorAttachments->size() : true);
	
	VkSubpassDescription desc{};
	desc.flags = 0;
	desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	desc.inputAttachmentCount = inputAttachments ? static_cast<uint32_t>(inputAttachments->size()) : 0;
	desc.pInputAttachments = inputAttachments ? inputAttachments->data() : nullptr;
	desc.colorAttachmentCount = colorAttachments ? static_cast<uint32_t>(colorAttachments->size()) : 0;
	desc.pColorAttachments = colorAttachments ? colorAttachments->data() : nullptr;
	desc.pResolveAttachments = resolveAttachments ? resolveAttachments->data() : nullptr;
	desc.pDepthStencilAttachment = depthStencilAttachment;
	desc.preserveAttachmentCount = preserveAttachments ? static_cast<uint32_t>(preserveAttachments->size()) : 0;
	desc.pPreserveAttachments = preserveAttachments ? preserveAttachments->data() : nullptr;
	
	return desc;
}

VkDescriptorSetLayoutCreateInfo get_descriptor_set_layout_create_info(vector<VkDescriptorSetLayoutBinding> const * const bindings)
{
	VkDescriptorSetLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = bindings ? static_cast<uint32_t>(bindings->size()) : 0;
	info.pBindings = bindings ? bindings->data() : nullptr;
	
	return info;
}

VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType const descriptorType, uint32_t const binding, VkShaderStageFlags const stageFlags, uint32_t const descriptorCount, vector<VkSampler> const * const immutableSamplers)
{
	assert(immutableSamplers ? immutableSamplers->size() == descriptorCount : true);
	
	VkDescriptorSetLayoutBinding info{};
	info.binding = binding;
	info.descriptorType = descriptorType;
	info.descriptorCount = descriptorCount;
	info.stageFlags = stageFlags;
	info.pImmutableSamplers = immutableSamplers ? immutableSamplers->data() : nullptr;
	
	return info;
}

VkPipelineLayoutCreateInfo get_pipeline_layout_create_info(vector<VkDescriptorSetLayout> const * const setLayouts, vector<VkPushConstantRange> const * const pushConstantRanges)
{
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.setLayoutCount = setLayouts ? static_cast<uint32_t>(setLayouts->size()) : 0;
	info.pSetLayouts = setLayouts ? setLayouts->data() : nullptr;
	info.pushConstantRangeCount = pushConstantRanges ? static_cast<uint32_t>(pushConstantRanges->size()) : 0;
	info.pPushConstantRanges = pushConstantRanges ? pushConstantRanges->data() : nullptr;
	
	return info;
}

VkGraphicsPipelineCreateInfo get_graphics_pipeline_create_info(VkRenderPass const renderPass, VkPipelineLayout const layout, uint32_t const subpass, VkPipelineCreateFlags const flags, vector<VkPipelineShaderStageCreateInfo> const * const shaderStages, VkPipelineVertexInputStateCreateInfo const * const  vertexInputState, VkPipelineInputAssemblyStateCreateInfo const * const inputAssemblyState, VkPipelineRasterizationStateCreateInfo const * const rasterizationState, VkPipelineColorBlendStateCreateInfo const * const colorBlendState, VkPipelineDepthStencilStateCreateInfo const * const depthStencilState, VkPipelineViewportStateCreateInfo const * const viewportState, VkPipelineMultisampleStateCreateInfo const * const multisampleState, VkPipelineTessellationStateCreateInfo const * const tesselationState, VkPipelineDynamicStateCreateInfo const * const dynamicState, VkPipeline const basePipelineHandle, int32_t const basePipelineIndex)
{
	assert(renderPass);
	assert(layout);
	assert(shaderStages && !shaderStages->empty());
	assert(vertexInputState);
	assert(inputAssemblyState);
	assert(rasterizationState);
	
	VkGraphicsPipelineCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.stageCount = static_cast<uint32_t>(shaderStages->size());
	info.pStages = shaderStages->data();
	info.pVertexInputState = vertexInputState;
	info.pInputAssemblyState = inputAssemblyState;
	info.pTessellationState = tesselationState;
	info.pViewportState = viewportState;
	info.pRasterizationState = rasterizationState;
	info.pMultisampleState = multisampleState;
	info.pDepthStencilState = depthStencilState;
	info.pColorBlendState = colorBlendState;
	info.pDynamicState = dynamicState;
	info.layout = layout;
	info.renderPass = renderPass;
	info.subpass = subpass;
	info.basePipelineHandle = basePipelineHandle;
	info.basePipelineIndex = basePipelineIndex;
	
	return info;
}

VkPushConstantRange get_push_constant_range(VkShaderStageFlags const stageFlags, uint32_t const size, uint32_t const offset)
{
	VkPushConstantRange range{};
	range.stageFlags = stageFlags;
	range.offset = offset;
	range.size = size;
	
	return range;
}

VkPipelineShaderStageCreateInfo get_pipeline_shader_stage_create_info(VkShaderStageFlagBits const stage, VkShaderModule const module, char const * const name, VkSpecializationInfo const * const specializationInfo)
{
	VkPipelineShaderStageCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.stage = stage;
	info.module = module;
	info.pName = name ? name : "main";
	info.pSpecializationInfo = specializationInfo;
	
	return info;
}

VkVertexInputBindingDescription get_vertex_input_binding_description(uint32_t const binding, uint32_t const stride, VkVertexInputRate const inputRate)
{
	VkVertexInputBindingDescription desc{};
	desc.binding = binding;
	desc.stride = stride;
	desc.inputRate = inputRate;
	
	return desc;
}

VkVertexInputAttributeDescription get_vertex_input_attribute_description(uint32_t const location, uint32_t const binding, VkFormat const format, uint32_t const offset)
{
	VkVertexInputAttributeDescription desc{};
	desc.location = location;
	desc.binding = binding;
	desc.format = format;
	desc.offset = offset;
	
	return desc;
}

VkPipelineVertexInputStateCreateInfo get_pipeline_vertex_input_state_create_info(vector<VkVertexInputBindingDescription> const * const vertexInputBindings, vector<VkVertexInputAttributeDescription> const  * const vertexInputAttributes)
{
	VkPipelineVertexInputStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.vertexBindingDescriptionCount = vertexInputBindings ? static_cast<uint32_t>(vertexInputBindings->size()) : 0;
	info.pVertexBindingDescriptions = vertexInputBindings ? vertexInputBindings->data() : nullptr;
	info.vertexAttributeDescriptionCount = vertexInputAttributes ? static_cast<uint32_t>(vertexInputAttributes->size()) : 0;
	info.pVertexAttributeDescriptions = vertexInputAttributes ? vertexInputAttributes->data() : nullptr;
	
	return info;
}

VkPipelineInputAssemblyStateCreateInfo get_pipeline_input_assembly_state_create_info(VkPrimitiveTopology const topology, VkBool32 const primitiveRestartEnable)
{
	VkPipelineInputAssemblyStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.topology = topology;
	info.primitiveRestartEnable = primitiveRestartEnable;
	
	return info;
}

VkPipelineRasterizationStateCreateInfo get_pipeline_rasterization_state_create_info(VkPolygonMode const polygonMode, VkCullModeFlags const cullMode, VkFrontFace const frontFace, VkBool32 const depthClampEnable, VkBool32 const rasterizerDiscardEnable, float const lineWidth, VkBool32 const depthBiasEnable, float const depthBiasConstantFactor, float const depthBiasClamp, float const depthBiasSlopeFactor)
{
	VkPipelineRasterizationStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.depthClampEnable = depthClampEnable;
	info.rasterizerDiscardEnable = rasterizerDiscardEnable;
	info.polygonMode = polygonMode;
	info.cullMode = cullMode;
	info.frontFace = frontFace;
	info.depthBiasEnable = depthBiasEnable;
	info.depthBiasConstantFactor = depthBiasConstantFactor;
	info.depthBiasClamp = depthBiasClamp;
	info.depthBiasSlopeFactor = depthBiasSlopeFactor;
	info.lineWidth = lineWidth;
	
	return info;
}

VkPipelineDynamicStateCreateInfo get_pipeline_dynamic_state_create_info(vector<VkDynamicState> const * const dynamicStates)
{
	assert(dynamicStates);
	assert(!dynamicStates->empty());
	
	VkPipelineDynamicStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.dynamicStateCount = static_cast<uint32_t>(dynamicStates->size());
	info.pDynamicStates = dynamicStates->data();
	
	return info;
}

VkPipelineViewportStateCreateInfo get_pipeline_viewport_state_create_info(vector<VkViewport> const * const viewports, vector<VkRect2D> const * const scissors)
{
	assert(viewports);
	assert(!viewports->empty());
	assert(scissors);
	assert(!scissors->empty());
	
	VkPipelineViewportStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.viewportCount = static_cast<uint32_t>(viewports->size());
	info.pViewports = viewports->data();
	info.scissorCount = static_cast<uint32_t>(scissors->size());
	info.pScissors = scissors->data();
	
	return info;
}

VkPipelineMultisampleStateCreateInfo get_pipeline_multisample_state_create_info(VkSampleCountFlagBits const rasterizationSamples, VkBool32 const sampleShadingEnable, float const minSampleShading, VkSampleMask const * const sampleMask, VkBool32 const alphaToCoverageEnable, VkBool32 const alphaToOneEnable)
{
	VkPipelineMultisampleStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.rasterizationSamples = rasterizationSamples;
	info.sampleShadingEnable = sampleShadingEnable;
	info.minSampleShading = minSampleShading;
	info.pSampleMask = sampleMask;
	info.alphaToCoverageEnable = alphaToCoverageEnable;
	info.alphaToOneEnable = alphaToOneEnable;
	
	return info;
}

VkPipelineTessellationStateCreateInfo get_pipeline_tessellation_state_create_info(uint32_t const patchControlPoints)
{
	VkPipelineTessellationStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.patchControlPoints = patchControlPoints;
	
	return info;
}

VkPipelineColorBlendAttachmentState get_pipeline_color_blend_attachment_state(VkBool32 const blendEnable, VkBlendFactor const srcColorBlendFactor, VkBlendFactor const dstColorBlendFactor, VkBlendOp const colorBlendOp, VkBlendFactor const srcAlphaBlendFactor, VkBlendFactor const dstAlphaBlendFactor, VkBlendOp const alphaBlendOp, VkColorComponentFlags const colorWriteMask)
{
	VkPipelineColorBlendAttachmentState state{};
	state.blendEnable = blendEnable;
	state.srcColorBlendFactor = srcColorBlendFactor;
	state.dstColorBlendFactor = dstColorBlendFactor;
	state.colorBlendOp = colorBlendOp;
	state.srcAlphaBlendFactor = srcAlphaBlendFactor;
	state.dstAlphaBlendFactor = dstAlphaBlendFactor;
	state.alphaBlendOp = alphaBlendOp;
	state.colorWriteMask = colorWriteMask;
	
	return state;
}

VkPipelineColorBlendStateCreateInfo get_pipeline_color_blend_state_create_info(vector<VkPipelineColorBlendAttachmentState> const * const attachments, VkBool32 const logicOpEnable, VkLogicOp const logicOp, vector<float> const blendConstants)
{
	assert(blendConstants.size() >= 4);
	
	VkPipelineColorBlendStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.logicOpEnable = logicOpEnable;
	info.logicOp = logicOp;
	info.attachmentCount = attachments ? static_cast<uint32_t>(attachments->size()) : 0;
	info.pAttachments = attachments ? attachments->data() : nullptr;
	info.blendConstants[0] = blendConstants[0];
	info.blendConstants[1] = blendConstants[1];
	info.blendConstants[2] = blendConstants[2];
	info.blendConstants[3] = blendConstants[3];
	
	return info;
}

} // namespace app::helpers