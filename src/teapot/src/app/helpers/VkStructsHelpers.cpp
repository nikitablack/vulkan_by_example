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

VkPipelineDepthStencilStateCreateInfo get_pipeline_depth_stencil_state_create_info(VkBool32 const depthTestEnable, VkBool32 const depthWriteEnable, VkCompareOp const depthCompareOp, VkBool32 const depthBoundsTestEnable, VkBool32 const stencilTestEnable, VkStencilOpState const front, VkStencilOpState const back, float const minDepthBounds, float const maxDepthBounds)
{
	VkPipelineDepthStencilStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.depthTestEnable = depthTestEnable;
	info.depthWriteEnable = depthWriteEnable;
	info.depthCompareOp = depthCompareOp;
	info.depthBoundsTestEnable = depthBoundsTestEnable;
	info.stencilTestEnable = stencilTestEnable;
	info.front = front;
	info.back = back;
	info.minDepthBounds = minDepthBounds;
	info.maxDepthBounds = maxDepthBounds;
	
	return info;
}

VkSwapchainCreateInfoKHR get_swap_chain_create_info(VkSurfaceKHR const surface, uint32_t const imageCount, VkFormat const imageFormat, VkColorSpaceKHR const imageColorSpace, VkExtent2D const imageExtent, VkSurfaceTransformFlagBitsKHR const preTransform, VkPresentModeKHR const presentMode, VkSharingMode const imageSharingMode, vector<uint32_t> const * const queueFamilyIndices, VkImageUsageFlags const imageUsage, uint32_t const imageArrayLayers, VkCompositeAlphaFlagBitsKHR const compositeAlpha, VkBool32 const clipped, VkSwapchainKHR const oldSwapchain)
{
	assert(imageUsage);
	assert(imageArrayLayers);
	
	VkSwapchainCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.pNext = nullptr;
	info.flags = 0;
	info.surface = surface;
	info.minImageCount = imageCount;
	info.imageFormat = imageFormat;
	info.imageColorSpace = imageColorSpace;
	info.imageExtent = imageExtent;
	info.imageArrayLayers = imageArrayLayers;
	info.imageUsage = imageUsage;
	info.imageSharingMode = imageSharingMode;
	info.queueFamilyIndexCount = queueFamilyIndices ? static_cast<uint32_t>(queueFamilyIndices->size()) : 0;
	info.pQueueFamilyIndices = queueFamilyIndices ? queueFamilyIndices->data() : nullptr;
	info.preTransform = preTransform;
	info.compositeAlpha = compositeAlpha;
	info.presentMode = presentMode;
	info.clipped = clipped;
	info.oldSwapchain = oldSwapchain;
	
	return info;
}

VkImageViewCreateInfo get_image_view_create_info(VkImage const image, VkImageViewType const viewType, VkFormat const format, VkImageSubresourceRange const subresourceRange, VkComponentMapping const components)
{
	VkImageViewCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.image = image;
	info.viewType = viewType;
	info.format = format;
	info.components = components;
	info.subresourceRange = subresourceRange;
	
	return info;
}

VkFramebufferCreateInfo get_frame_buffer_create_info(VkRenderPass const renderPass, uint32_t const width, uint32_t const height, vector<VkImageView > const * const attachments, uint32_t const layers)
{
	assert(renderPass);
	assert(width);
	assert(height);
	assert(height);
	
	VkFramebufferCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.renderPass = renderPass;
	info.attachmentCount = attachments ? static_cast<uint32_t>(attachments->size()) : 0;
	info.pAttachments = attachments ? attachments->data() : nullptr;
	info.width = width;
	info.height = height;
	info.layers = layers;
	
	return info;
}

VkBufferCreateInfo get_buffer_create_info(VkDeviceSize const size, VkBufferUsageFlags const usage, VkSharingMode const sharingMode, vector<uint32_t> const * const queueFamilyIndices, VkBufferCreateFlags const flags, void const * const next)
{
	assert(size);
	assert(usage);
	assert(sharingMode == VK_SHARING_MODE_CONCURRENT ? queueFamilyIndices != nullptr && !queueFamilyIndices->empty() : true);
	
	VkBufferCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.pNext = next;
	info.flags = flags;
	info.size = size;
	info.usage = usage;
	info.sharingMode = sharingMode;
	info.queueFamilyIndexCount = queueFamilyIndices ? static_cast<uint32_t>(queueFamilyIndices->size()) : 0;
	info.pQueueFamilyIndices = queueFamilyIndices ? queueFamilyIndices->data() : nullptr;
	
	return info;
}

VkMemoryAllocateInfo get_memory_allocate_info(VkDeviceSize const allocationSize, uint32_t const memoryTypeIndex, void const * const next)
{
	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.pNext = next;
	info.allocationSize = allocationSize;
	info.memoryTypeIndex = memoryTypeIndex;
	
	return info;
}

VkCommandPoolCreateInfo get_command_pool_create_info(uint32_t const queueFamilyIndex, VkCommandPoolCreateFlags const flags)
{
	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.queueFamilyIndex = queueFamilyIndex;
	
	return info;
}

VkCommandBufferAllocateInfo get_command_buffer_allocate_info(VkCommandPool const commandPool, uint32_t const commandBufferCount, VkCommandBufferLevel const level)
{
	VkCommandBufferAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.commandPool = commandPool;
	info.level = level;
	info.commandBufferCount = commandBufferCount;
	
	return info;
}

VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags const flags, VkCommandBufferInheritanceInfo const * const inheritanceInfo)
{
	VkCommandBufferBeginInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.pInheritanceInfo = inheritanceInfo;
	
	return info;
}

VkBufferCopy get_buffer_copy(VkDeviceSize const size, VkDeviceSize const srcOffset, VkDeviceSize const dstOffset)
{
	VkBufferCopy copy{};
	copy.srcOffset = srcOffset;
	copy.dstOffset = dstOffset;
	copy.size = size;
	
	return copy;
}

VkBufferMemoryBarrier get_buffer_memory_barrier(VkBuffer const buffer, VkAccessFlags const srcAccessMask, VkAccessFlags const dstAccessMask, VkDeviceSize const size, VkDeviceSize const offset, uint32_t const srcQueueFamilyIndex, uint32_t const dstQueueFamilyIndex)
{
	VkBufferMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = srcAccessMask;
	barrier.dstAccessMask = dstAccessMask;
	barrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
	barrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
	barrier.buffer = buffer;
	barrier.offset = offset;
	barrier.size = size;
	
	return barrier;
}

VkSubmitInfo get_submit_info(vector<VkCommandBuffer> const * const commandBuffers, vector<VkSemaphore> const * const waitSemaphores, vector<VkPipelineStageFlags> const * const waitStages, vector<VkSemaphore> const * const signalSemaphores)
{
	assert(commandBuffers);
	assert(waitSemaphores ? (waitStages && waitSemaphores->size() == waitStages->size()) : true);
	
	VkSubmitInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.pNext = nullptr;
	info.waitSemaphoreCount = waitSemaphores ? static_cast<uint32_t>(waitSemaphores->size()) : 0;
	info.pWaitSemaphores = waitSemaphores ? waitSemaphores->data() : nullptr;
	info.pWaitDstStageMask = waitStages ? waitStages->data() : nullptr;
	info.commandBufferCount = static_cast<uint32_t>(commandBuffers->size());
	info.pCommandBuffers = commandBuffers->data();
	info.signalSemaphoreCount = signalSemaphores ? static_cast<uint32_t>(signalSemaphores->size()) : 0;
	info.pSignalSemaphores = signalSemaphores ? signalSemaphores->data() : nullptr;
	
	return info;
}

VkDescriptorPoolCreateInfo get_descriptor_pool_create_info(uint32_t const maxSets, vector<VkDescriptorPoolSize> const * const poolSizes)
{
	assert(poolSizes && !poolSizes->empty());
	
	VkDescriptorPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.maxSets = maxSets;
	info.poolSizeCount = static_cast<uint32_t>(poolSizes->size());
	info.pPoolSizes = poolSizes->data();
	
	return info;
}

VkDescriptorPoolSize get_descriptor_pool_size(VkDescriptorType const type, uint32_t const descriptorCount)
{
	VkDescriptorPoolSize size{};
	size.type = type;
	size.descriptorCount = descriptorCount;
	
	return size;
}

VkDescriptorSetAllocateInfo get_descriptor_set_allocate_info(VkDescriptorPool const descriptorPool, vector<VkDescriptorSetLayout> const * const layouts)
{
	assert(layouts && !layouts->empty());
	
	VkDescriptorSetAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.descriptorPool = descriptorPool;
	info.descriptorSetCount = static_cast<uint32_t>(layouts->size());
	info.pSetLayouts = layouts->data();
	
	return info;
}

VkDescriptorBufferInfo get_descriptor_buffer_info(VkBuffer const buffer, VkDeviceSize const range, VkDeviceSize const offset)
{
	VkDescriptorBufferInfo info{};
	info.buffer = buffer;
	info.offset = offset;
	info.range = range;
	
	return info;
}

VkWriteDescriptorSet get_write_descriptor_set(VkDescriptorSet const dstSet, uint32_t const dstBinding, VkDescriptorType const descriptorType, vector<VkDescriptorBufferInfo> const * const bufferInfos)
{
	assert(bufferInfos && !bufferInfos->empty());
	
	VkWriteDescriptorSet set{};
	set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	set.pNext = nullptr;
	set.dstSet = dstSet;
	set.dstBinding = dstBinding;
	set.dstArrayElement = 0;
	set.descriptorCount = static_cast<uint32_t>(bufferInfos->size());
	set.descriptorType = descriptorType;
	set.pImageInfo = nullptr;
	set.pBufferInfo = bufferInfos->data();
	set.pTexelBufferView = nullptr;
	
	return set;
}

VkSemaphoreCreateInfo get_semaphore_create_info()
{
	VkSemaphoreCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	
	return info;
}

VkFenceCreateInfo get_fence_create_info(VkFenceCreateFlags const flags)
{
	VkFenceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	
	return info;
}

VkRenderPassBeginInfo get_render_pass_begin_info(VkRenderPass const renderPass, VkFramebuffer const framebuffer, VkRect2D const renderArea, vector<VkClearValue> const * const clearValues)
{
	VkRenderPassBeginInfo info{};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.pNext = nullptr;
	info.renderPass = renderPass;
	info.framebuffer = framebuffer;
	info.renderArea = renderArea;
	info.clearValueCount = clearValues ? static_cast<uint32_t>(clearValues->size()) : 0;
	info.pClearValues = clearValues ? clearValues->data() : nullptr;
	
	return info;
}

VkPresentInfoKHR get_present_info(vector<VkSwapchainKHR> const * const swapChains, vector<uint32_t> const * const imageIndices, vector<VkSemaphore> const * const signalSemaphores, vector<VkResult> * const results)
{
	assert(swapChains && imageIndices);
	assert(!swapChains->empty());
	assert(swapChains->size() == imageIndices->size());
	assert(results ? results->size() == swapChains->size() : true);
	
	VkPresentInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.pNext = nullptr;
	info.waitSemaphoreCount = signalSemaphores ? static_cast<uint32_t>(signalSemaphores->size()) : 0;
	info.pWaitSemaphores = signalSemaphores ? signalSemaphores->data() : nullptr;
	info.swapchainCount = static_cast<uint32_t>(swapChains->size());
	info.pSwapchains = swapChains->data();
	info.pImageIndices = imageIndices->data();
	info.pResults = results ? results->data() : nullptr;
	
	return info;
}

VkMappedMemoryRange get_mapped_memory_range(VkDeviceMemory const memory, VkDeviceSize const size, VkDeviceSize const offset)
{
	VkMappedMemoryRange range{};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;
	range.memory = memory;
	range.offset = offset;
	range.size = size;
	
	return range;
}

VkImageCreateInfo get_image_create_info(VkExtent3D const extent, VkFormat const format, VkImageUsageFlags const usage, VkImageType const imageType, VkImageCreateFlags const flags, VkImageLayout const initialLayout, VkImageTiling const tiling, uint32_t const mipLevels, uint32_t const arrayLayers, vector<uint32_t> const * const queueFamilyIndices, VkSampleCountFlagBits const samples, VkSharingMode const sharingMode)
{
	VkImageCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.imageType = imageType;
	info.format = format;
	info.extent = extent;
	info.mipLevels = mipLevels;
	info.arrayLayers = arrayLayers;
	info.samples = samples;
	info.tiling = tiling;
	info.usage = usage;
	info.sharingMode = sharingMode;
	info.queueFamilyIndexCount = queueFamilyIndices ? static_cast<uint32_t>(queueFamilyIndices->size()) : 0;
	info.pQueueFamilyIndices = queueFamilyIndices ? queueFamilyIndices->data() : nullptr;
	info.initialLayout = initialLayout;
	
	return info;
}

VkImageMemoryBarrier get_image_memory_barrier(VkImage const image, VkAccessFlags const srcAccessMask, VkAccessFlags const dstAccessMask, VkImageLayout const oldLayout, VkImageLayout const newLayout, VkImageSubresourceRange const subresourceRange, uint32_t const srcQueueFamilyIndex, uint32_t const dstQueueFamilyIndex)
{
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = srcAccessMask;
	barrier.dstAccessMask = dstAccessMask;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
	barrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
	barrier.image = image;
	barrier.subresourceRange = subresourceRange;
	
	return barrier;
}

} // namespace app::helpers