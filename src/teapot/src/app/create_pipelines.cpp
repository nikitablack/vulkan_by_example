#include "helpers/VkObjectsHelpers.h"
#include "helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

using namespace std;

namespace app
{

MaybeAppData create_pipelines(AppData data)
{
	assert(data.window);
	assert(data.vertexShaderModule);
	assert(data.tessControlShaderModule);
	assert(data.tessEvaluationShaderModule);
	assert(data.fragmentShaderModule);
	assert(data.renderPass);
	assert(data.pipelineLayout);
	assert(data.device);
	
	VkPipelineShaderStageCreateInfo const vertexShaderStage{helpers::get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, data.vertexShaderModule)};
	VkPipelineShaderStageCreateInfo const tesselationControlShaderStage{helpers::get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, data.tessControlShaderModule)};
	VkPipelineShaderStageCreateInfo const tesselationEvaluationShaderStage{helpers::get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, data.tessEvaluationShaderModule)};
	VkPipelineShaderStageCreateInfo const fragmentShaderStage{helpers::get_pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, data.fragmentShaderModule)};
	
	vector<VkPipelineShaderStageCreateInfo> const shaderStages{vertexShaderStage, tesselationControlShaderStage, tesselationEvaluationShaderStage, fragmentShaderStage};
	
	vector<VkVertexInputBindingDescription> const vertexInputBindings{helpers::get_vertex_input_binding_description(0, sizeof(float) * 3)};
	vector<VkVertexInputAttributeDescription> const vertexInputAttributes{helpers::get_vertex_input_attribute_description(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0)};
	
	VkPipelineVertexInputStateCreateInfo const vertexInputState{helpers::get_pipeline_vertex_input_state_create_info(&vertexInputBindings, &vertexInputAttributes)};
	VkPipelineInputAssemblyStateCreateInfo const inputAssemblyState{helpers::get_pipeline_input_assembly_state_create_info(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST) };
	VkPipelineRasterizationStateCreateInfo const rasterizationStateWireframe{helpers::get_pipeline_rasterization_state_create_info(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE)};
	
	std::vector<VkPipelineColorBlendAttachmentState> const colorBlendAttachments{helpers::get_pipeline_color_blend_attachment_state()};
	VkPipelineColorBlendStateCreateInfo const colorBlendState{helpers::get_pipeline_color_blend_state_create_info(&colorBlendAttachments)};
	
	vector<VkViewport> const viewports(1);
	vector<VkRect2D> const scissors(1);
	VkPipelineViewportStateCreateInfo const viewportState{helpers::get_pipeline_viewport_state_create_info(&viewports, &scissors)};
	
	VkPipelineMultisampleStateCreateInfo const multisampleState{helpers::get_pipeline_multisample_state_create_info()};
	
	VkPipelineTessellationStateCreateInfo const tesselationState{helpers::get_pipeline_tessellation_state_create_info(16)};
	
	vector<VkDynamicState> const dynamicStates{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo const dynamicState{helpers::get_pipeline_dynamic_state_create_info(&dynamicStates)};
	
	helpers::MaybePipeline const mbWireframePipeline{helpers::create_pipeline(data.device, data.renderPass, data.pipelineLayout, 0, VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT, &shaderStages, &vertexInputState, &inputAssemblyState, &rasterizationStateWireframe, &colorBlendState, nullptr, &viewportState, &multisampleState, &tesselationState, &dynamicState)};
	if(!mbWireframePipeline)
		tl::make_unexpected(mbWireframePipeline.error());
	
	data.wireframePipeline = *mbWireframePipeline;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(data.wireframePipeline), "wireframe graphics pipeline");
	
	VkPipelineRasterizationStateCreateInfo const rasterizationStateSolid{helpers::get_pipeline_rasterization_state_create_info(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT)};
	
	helpers::MaybePipeline const mbSolidPipeline{helpers::create_pipeline(data.device, data.renderPass, data.pipelineLayout, 0, VK_PIPELINE_CREATE_DERIVATIVE_BIT, &shaderStages, &vertexInputState, &inputAssemblyState, &rasterizationStateSolid, &colorBlendState, nullptr, &viewportState, &multisampleState, &tesselationState, &dynamicState, data.wireframePipeline)};
	if(!mbSolidPipeline)
		tl::make_unexpected(mbSolidPipeline.error());
	
	data.solidPipeline = *mbSolidPipeline;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(data.solidPipeline), "solid graphics pipeline");
	
	return data;
}

} // namespace app