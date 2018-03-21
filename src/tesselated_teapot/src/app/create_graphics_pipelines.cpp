#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_graphics_pipelines(AppData appData)
	{
		assert(appData.window);
		assert(appData.vertexShaderModule);
		assert(appData.tessControlShaderModule);
		assert(appData.tessEvaluationShaderModule);
		assert(appData.fragmentShaderModule);
		assert(appData.renderPass);
		assert(appData.pipelineLayout);
		assert(appData.device);
		
		VkPipelineShaderStageCreateInfo const vertexShaderStage{ get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, appData.vertexShaderModule) };
		VkPipelineShaderStageCreateInfo const tesselationControlShaderStage{ get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, appData.tessControlShaderModule)};
		VkPipelineShaderStageCreateInfo const tesselationEvaluationShaderStage{ get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, appData.tessEvaluationShaderModule)};
		VkPipelineShaderStageCreateInfo const fragmentShaderStage{ get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, appData.fragmentShaderModule) };
		
		vector<VkPipelineShaderStageCreateInfo> const shaderStages{vertexShaderStage, tesselationControlShaderStage, tesselationEvaluationShaderStage, fragmentShaderStage};
		
		vector<VkVertexInputBindingDescription> const vertexInputBindings{get_vertex_input_binding_description(0, sizeof(float) * 3)};
		vector<VkVertexInputAttributeDescription> const vertexInputAttributes{ get_vertex_input_attribute_description(0, 0, VK_FORMAT_R32G32B32_SFLOAT)};
		
		VkPipelineVertexInputStateCreateInfo const vertexInputState{ get_pipeline_vertex_input_state_create_info(&vertexInputBindings, &vertexInputAttributes) };
		VkPipelineInputAssemblyStateCreateInfo const inputAssemblyState{ get_pipeline_input_assembly_state_create_info(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST) };
		VkPipelineRasterizationStateCreateInfo const rasterizationStateWireframe{get_pipeline_rasterization_state_create_info(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE)};
		
		vector<VkViewport> const viewports{get_viewport(static_cast<float>(appData.surfaceExtent.width), static_cast<float>(appData.surfaceExtent.height))};
		vector<VkRect2D> const scissors{get_scissor(appData.surfaceExtent)};
		VkPipelineViewportStateCreateInfo const viewportState{get_pipeline_viewport_state_create_info(&viewports, &scissors)};
		
		VkPipelineMultisampleStateCreateInfo const multisampleState{get_pipeline_multisample_state_create_info()};
		
		VkPipelineTessellationStateCreateInfo const tesselationState{ get_pipeline_tessellation_state_create_info(16) };
		
		std::vector<VkPipelineColorBlendAttachmentState> const colorBlendAttachments{get_pipeline_color_blend_attachment_state()};
		VkPipelineColorBlendStateCreateInfo const colorBlendState{get_pipeline_color_blend_state_create_info(&colorBlendAttachments)};
		
		VkGraphicsPipelineCreateInfo pipelineInfoWireframe{get_graphics_pipeline_create_info(appData.renderPass,
		                                                                                     appData.pipelineLayout,
		                                                                                     &shaderStages,
		                                                                                     &vertexInputState,
		                                                                                     &inputAssemblyState,
		                                                                                     &rasterizationStateWireframe,
		                                                                                     &viewportState,
		                                                                                     &multisampleState,
		                                                                                     &tesselationState,
		                                                                                     &colorBlendState,
		                                                                                     nullptr,
		                                                                                     VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT)};
		
		if (vkCreateGraphicsPipelines(appData.device, VK_NULL_HANDLE, 1, &pipelineInfoWireframe, nullptr, &appData.graphicsPipelineWireframe) != VK_SUCCESS)
			return make_unexpected("failed to create graphics pipeline");
		
		VkPipelineRasterizationStateCreateInfo const rasterizationStateSolid{get_pipeline_rasterization_state_create_info(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE)};
		
		VkGraphicsPipelineCreateInfo pipelineInfoSolid{pipelineInfoWireframe};
		pipelineInfoSolid.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
		pipelineInfoSolid.pRasterizationState = &rasterizationStateSolid;
		pipelineInfoSolid.basePipelineHandle = appData.graphicsPipelineWireframe;
		
		if (vkCreateGraphicsPipelines(appData.device, VK_NULL_HANDLE, 1, &pipelineInfoSolid, nullptr, &appData.graphicsPipelineSolid) != VK_SUCCESS)
			return make_unexpected("failed to create graphics pipeline");
		
		return appData;
	}
}