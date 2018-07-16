#include "helpers/VkObjectsHelpers.h"
#include "helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_pipeline_layout(AppData data)
{
	assert(data.descriptorSetLayout);
	assert(data.device);

	std::vector<VkDescriptorSetLayout> const setLayouts{data.descriptorSetLayout};
	std::vector<VkPushConstantRange> const pushConstantRanges{helpers::get_push_constant_range(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, sizeof(data.tessLevel))};
	
	helpers::MaybePipelineLayout const mbPipelineLayout{helpers::create_pipeline_layout(data.device, &setLayouts, &pushConstantRanges)};
	if(!mbPipelineLayout)
		return tl::make_unexpected(mbPipelineLayout.error());
	
	data.pipelineLayout = *mbPipelineLayout;
	
	return data;
}

} // namespace app