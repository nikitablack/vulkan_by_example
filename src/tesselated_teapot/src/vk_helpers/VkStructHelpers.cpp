#include "VkStructHelpers.h"

#include <cassert>
#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk_helpers
{
	VkInstanceCreateInfo get_instance_create_info(vector<char const *> const * const extensions,
	                                              vector<char const *> const * const layers)
	{
		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.pApplicationInfo = nullptr;
		info.enabledLayerCount = (layers != nullptr) ? static_cast<uint32_t>(layers->size()) : 0;
		info.ppEnabledLayerNames = (layers != nullptr) ? layers->data() : nullptr;
		info.enabledExtensionCount = (extensions != nullptr) ? static_cast<uint32_t>(extensions->size()) : 0;
		info.ppEnabledExtensionNames = (extensions != nullptr) ? extensions->data() : nullptr;
		
		return info;
	}
	
	VkDeviceQueueCreateInfo get_device_queue_create_info(uint32_t const queueFamilyIndex, vector<float> const * const queuePriorities)
	{
		VkDeviceQueueCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.queueFamilyIndex = queueFamilyIndex;
		info.queueCount = (queuePriorities != nullptr) ? static_cast<uint32_t>(queuePriorities->size()) : 0;
		info.pQueuePriorities = (queuePriorities != nullptr) ? queuePriorities->data() : nullptr;
		
		return info;
	}
	
	VkDeviceCreateInfo get_device_create_info(vector<VkDeviceQueueCreateInfo> const * const queueCreateInfos,
	                                          vector<char const *> const * const deviceExtensions,
	                                          VkPhysicalDeviceFeatures const * const deviceFeatures)
	{
		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos->size());
		info.pQueueCreateInfos = queueCreateInfos->data();
		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;
		info.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions->size());
		info.ppEnabledExtensionNames = deviceExtensions->data();
		info.pEnabledFeatures = deviceFeatures;
		
		return info;
	}
	
	VkShaderModuleCreateInfo get_shader_module_create_info(vector<char> const * const shaderCode)
	{
		VkShaderModuleCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.codeSize = shaderCode->size();
		info.pCode = reinterpret_cast<uint32_t const *>(shaderCode->data());
		
		return info;
	}
	
	VkDebugReportCallbackCreateInfoEXT get_debug_report_callback_createInfo(PFN_vkDebugReportCallbackEXT const callback)
	{
		VkDebugReportCallbackCreateInfoEXT info{};
		info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		info.pNext = nullptr;
		info.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		info.pfnCallback = callback;
		info.pUserData = nullptr;
		
		return info;
	}
	
	VkAttachmentDescription get_attachment_description(VkFormat const format, VkImageLayout const initialLayout, VkImageLayout const finalLayout)
	{
		VkAttachmentDescription desc{};
		desc.flags = 0;
		desc.format = format;
		desc.samples = VK_SAMPLE_COUNT_1_BIT;
		desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
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
	
	VkSubpassDescription get_subpass_description(vector<VkAttachmentReference> const * const colorAttachments,
	                                             VkAttachmentReference const * const depthStencilAttachment)
	{
		VkSubpassDescription desc{};
		desc.flags = 0;
		desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		desc.inputAttachmentCount = 0;
		desc.pInputAttachments = nullptr;
		desc.colorAttachmentCount = colorAttachments ? static_cast<uint32_t>(colorAttachments->size()) : 0;
		desc.pColorAttachments = colorAttachments ? colorAttachments->data() : nullptr;
		desc.pResolveAttachments = nullptr;
		desc.pDepthStencilAttachment = depthStencilAttachment;
		desc.preserveAttachmentCount = 0;
		desc.pPreserveAttachments = nullptr;
		
		return desc;
	}
	
	VkRenderPassCreateInfo get_render_pass_create_info(vector<VkSubpassDescription> const * const subpasses,
	                                                   vector<VkAttachmentDescription> const * const attachments,
	                                                   vector<VkSubpassDependency> const * const dependencies)
	{
		assert(subpasses != nullptr && !subpasses->empty());
		
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
	
	VkDescriptorSetLayoutBinding get_descriptor_set_layout_binding(VkDescriptorType const descriptorType,
	                                                               uint32_t const binding,
	                                                               VkShaderStageFlags const stageFlags,
	                                                               uint32_t const descriptorCount)
	{
		VkDescriptorSetLayoutBinding info{};
		info.binding = binding;
		info.descriptorType = descriptorType;
		info.descriptorCount = descriptorCount;
		info.stageFlags = stageFlags;
		info.pImmutableSamplers = nullptr;
		
		return info;
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
	
	VkPushConstantRange get_push_constant_range(VkShaderStageFlags const stageFlags, uint32_t const size, uint32_t const offset)
	{
		VkPushConstantRange range{};
		range.stageFlags = stageFlags;
		range.offset = offset;
		range.size = size;
		
		return range;
	}
	
	VkPipelineLayoutCreateInfo get_pipeline_layout_create_info(vector<VkDescriptorSetLayout> const * const setLayouts,
	                                                           vector<VkPushConstantRange> const * const pushConstantRanges)
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
	
	VkPipelineShaderStageCreateInfo get_pipeline_shader_stage_create_info(VkShaderStageFlagBits const stage, VkShaderModule const module)
	{
		VkPipelineShaderStageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.stage = stage;
		info.module = module;
		info.pName = "main";
		info.pSpecializationInfo = nullptr;
		
		return info;
	}
	
	VkVertexInputBindingDescription get_vertex_input_binding_description(uint32_t const binding, uint32_t const stride)
	{
		VkVertexInputBindingDescription desc{};
		desc.binding = binding;
		desc.stride = stride;
		desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		
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
	
	VkPipelineVertexInputStateCreateInfo get_pipeline_vertex_input_state_create_info(vector<VkVertexInputBindingDescription> const * const vertexInputBindings,
	                                                                                 vector<VkVertexInputAttributeDescription> const  * const vertexInputAttributes)
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
	
	VkPipelineInputAssemblyStateCreateInfo get_pipeline_input_assembly_state_create_info(VkPrimitiveTopology const topology)
	{
		VkPipelineInputAssemblyStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.topology = topology;
		info.primitiveRestartEnable = VK_FALSE;
		
		return info;
	}
	
	VkPipelineRasterizationStateCreateInfo get_pipeline_rasterization_state_create_info(VkPolygonMode const polygonMode,
	                                                                                    VkCullModeFlags const cullMode)
	{
		VkPipelineRasterizationStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.depthClampEnable = VK_FALSE;
		info.rasterizerDiscardEnable = VK_FALSE;
		info.polygonMode = polygonMode;
		info.cullMode = cullMode;
		info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.depthBiasEnable = VK_FALSE;
		info.depthBiasConstantFactor = 0.0f;
		info.depthBiasClamp = 0.0f;
		info.depthBiasSlopeFactor = 0.0f;
		info.lineWidth = 1.0f;
		
		return info;
	}
	
	VkViewport get_viewport(float const width, float const height)
	{
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		
		return viewport;
	}
	
	VkRect2D get_scissor(VkExtent2D const extent)
	{
		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = extent;
		
		return scissor;
	}
	
	VkPipelineViewportStateCreateInfo get_pipeline_viewport_state_create_info(vector<VkViewport> const * const viewports,
	                                                                          vector<VkRect2D> const * const scissors)
	{
		assert(viewports && !viewports->empty());
		assert(scissors && !scissors->empty());
		
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
	
	VkPipelineMultisampleStateCreateInfo get_pipeline_multisample_state_create_info()
	{
		VkPipelineMultisampleStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.sampleShadingEnable = VK_FALSE;
		info.minSampleShading = 1.0f;
		info.pSampleMask = nullptr;
		info.alphaToCoverageEnable = VK_FALSE;
		info.alphaToOneEnable = VK_FALSE;
		
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
	
	VkPipelineColorBlendAttachmentState get_pipeline_color_blend_attachment_state()
	{
		VkPipelineColorBlendAttachmentState state{};
		state.blendEnable = VK_FALSE;
		state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		state.colorBlendOp = VK_BLEND_OP_ADD;
		state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		state.alphaBlendOp = VK_BLEND_OP_ADD;
		state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		
		return state;
	}
	
	VkPipelineColorBlendStateCreateInfo get_pipeline_color_blend_state_create_info(vector<VkPipelineColorBlendAttachmentState> const * const colorBlendAttachments)
	{
		VkPipelineColorBlendStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.logicOpEnable = VK_FALSE;
		info.logicOp = VK_LOGIC_OP_COPY;
		info.attachmentCount = colorBlendAttachments ? static_cast<uint32_t>(colorBlendAttachments->size()) : 0;
		info.pAttachments = colorBlendAttachments ? colorBlendAttachments->data() : nullptr;
		info.blendConstants[0] = 0.0f;
		info.blendConstants[1] = 0.0f;
		info.blendConstants[2] = 0.0f;
		info.blendConstants[3] = 0.0f;
		
		return info;
	}
	
	VkGraphicsPipelineCreateInfo get_graphics_pipeline_create_info(VkRenderPass const renderPass,
	                                                               VkPipelineLayout const pipelineLayout,
	                                                               vector<VkPipelineShaderStageCreateInfo> const * const shaderStages,
	                                                               VkPipelineVertexInputStateCreateInfo const * const  vertexInputState,
	                                                               VkPipelineInputAssemblyStateCreateInfo const * const inputAssemblyState,
	                                                               VkPipelineRasterizationStateCreateInfo const * const rasterizationState,
	                                                               VkPipelineViewportStateCreateInfo const * const viewportState,
	                                                               VkPipelineMultisampleStateCreateInfo const * const multisampleState,
	                                                               VkPipelineTessellationStateCreateInfo const * const tesselationState,
	                                                               VkPipelineColorBlendStateCreateInfo const * const colorBlendState,
	                                                               VkPipelineDepthStencilStateCreateInfo const * const depthStencilState,
	                                                               VkPipelineCreateFlags const flags,
	                                                               VkPipeline const basePipelineHandle)
	{
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
		info.pDynamicState = nullptr;
		info.layout = pipelineLayout;
		info.renderPass = renderPass;
		info.subpass = 0;
		info.basePipelineHandle = basePipelineHandle;
		info.basePipelineIndex = -1;
		
		return info;
	}
	
	VkSwapchainCreateInfoKHR get_swap_chain_create_info(VkSurfaceKHR const surface,
	                                                    uint32_t const imageCount,
	                                                    VkFormat const imageFormat,
	                                                    VkColorSpaceKHR const imageColorSpace,
	                                                    VkExtent2D const imageExtent,
	                                                    VkSurfaceTransformFlagBitsKHR const preTransform,
	                                                    VkPresentModeKHR const presentMode)
	{
		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.flags = 0;
		info.surface = surface;
		info.minImageCount = imageCount;
		info.imageFormat = imageFormat;
		info.imageColorSpace = imageColorSpace;
		info.imageExtent = imageExtent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
		info.preTransform = preTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = presentMode;
		info.clipped = VK_TRUE;
		info.oldSwapchain = VK_NULL_HANDLE;
		
		return info;
	}
	
	VkImageViewCreateInfo get_image_view_create_info(VkImage const image, VkFormat const format, VkImageAspectFlags const imageAspectFlags)
	{
		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.image = image;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = format;
		info.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		info.subresourceRange = { imageAspectFlags, 0, 1, 0, 1 };
		
		return info;
	}
	
	VkFramebufferCreateInfo get_frame_buffer_create_info(VkRenderPass const renderPass,
	                                                     uint32_t const width,
	                                                     uint32_t const height,
	                                                     vector<VkImageView > const * const attachments)
	{
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.renderPass = renderPass;
		info.attachmentCount = attachments ? static_cast<uint32_t>(attachments->size()) : 0;
		info.pAttachments = attachments ? attachments->data() : nullptr;
		info.width = width;
		info.height = height;
		info.layers = 1;
		
		return info;
	}
	
	VkBufferCreateInfo get_buffer_create_info(VkDeviceSize const size, VkBufferUsageFlags const usage)
	{
		VkBufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.size = size;
		info.usage = usage;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
		
		return info;
	}
	
	VkMemoryAllocateInfo get_memory_allocate_info(VkDeviceSize const allocationSize, uint32_t const memoryTypeIndex)
	{
		VkMemoryAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.allocationSize = allocationSize;
		info.memoryTypeIndex = memoryTypeIndex;
		
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
	
	VkCommandPoolCreateInfo get_command_pool_create_info(uint32_t const queueFamilyIndex, VkCommandPoolCreateFlags const flags)
	{
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = flags;
		info.queueFamilyIndex = queueFamilyIndex;
		
		return info;
	}
	
	VkCommandBufferAllocateInfo get_command_buffer_allocate_info(VkCommandPool const commandPool, uint32_t const commandBufferCount)
	{
		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.commandPool = commandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = commandBufferCount;
		
		return info;
	}
	
	VkCommandBufferBeginInfo get_command_buffer_begin_info(VkCommandBufferUsageFlags const flags)
	{
		VkCommandBufferBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext = nullptr;
		info.flags = flags;
		info.pInheritanceInfo = nullptr;
		
		return info;
	}
	
	VkBufferMemoryBarrier get_buffer_memory_barrier(VkBuffer const buffer,
	                                                VkAccessFlags const srcAccessMask,
	                                                VkAccessFlags const dstAccessMask,
	                                                VkDeviceSize const size,
	                                                VkDeviceSize const offset,
	                                                uint32_t const srcQueueFamilyIndex,
	                                                uint32_t const dstQueueFamilyIndex)
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
	
	VkBufferCopy get_buffer_copy(VkDeviceSize const size)
	{
		VkBufferCopy copy{};
		copy.srcOffset = 0;
		copy.dstOffset = 0;
		copy.size = size;
		
		return copy;
	}
	
	VkSubmitInfo get_submit_info(vector<VkCommandBuffer> const * const commandBuffers,
	                             vector<VkSemaphore> const * const waitSemaphores,
	                             vector<VkPipelineStageFlags> const * const waitStages,
	                             vector<VkSemaphore> const * const signalSemaphores)
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
	
	VkDescriptorPoolSize get_descriptor_pool_size(VkDescriptorType const type, uint32_t const descriptorCount)
	{
		VkDescriptorPoolSize size{};
		size.type = type;
		size.descriptorCount = descriptorCount;
		
		return size;
	}
	
	VkDescriptorPoolCreateInfo get_descriptor_pool_create_info(uint32_t const maxSets, vector<VkDescriptorPoolSize> const * const pollSizes)
	{
		assert(pollSizes && pollSizes->size() > 0);
		
		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.maxSets = maxSets;
		info.poolSizeCount = static_cast<uint32_t>(pollSizes->size());
		info.pPoolSizes = pollSizes->data();
		
		return info;
	}
	
	VkDescriptorSetAllocateInfo get_descriptor_set_allocate_info(VkDescriptorPool const descriptorPool, vector<VkDescriptorSetLayout> const * const layouts)
	{
		assert(layouts && layouts->size() > 0);
		
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
	
	VkWriteDescriptorSet get_write_descriptor_set(VkDescriptorSet const dstSet,
	                                              uint32_t const dstBinding,
	                                              VkDescriptorType const descriptorType,
	                                              vector<VkDescriptorBufferInfo> const * const bufferInfos)
	{
		assert(bufferInfos && bufferInfos->size() > 0);
		
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
}