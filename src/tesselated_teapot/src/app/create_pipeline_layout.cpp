#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_pipeline_layout(AppData appData)
	{
		assert(appData.descriptorSetLayout);
		assert(appData.device);
	
		vector<VkDescriptorSetLayout> const setLayouts{appData.descriptorSetLayout};
		vector<VkPushConstantRange> const pushConstantRanges{get_push_constant_range(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, sizeof(appData.tessLevel))};
		
		VkPipelineLayoutCreateInfo const pipelineLayoutInfo{get_pipeline_layout_create_info(&setLayouts, &pushConstantRanges)};
		
		if (vkCreatePipelineLayout(appData.device, &pipelineLayoutInfo, nullptr, &appData.pipelineLayout) != VK_SUCCESS)
			return make_unexpected("failed to create pipeline layout");
		
		return appData;
	}
}