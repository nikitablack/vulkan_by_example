#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_descriptor_set_layout(AppData appData)
	{
		assert(appData.device);
	
		vector<VkDescriptorSetLayoutBinding> const bindings{get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT),
		                                                    get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT),
		                                                    get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT),
		                                                    get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) };
		
		VkDescriptorSetLayoutCreateInfo const layoutInfo{get_descriptor_set_layout_create_info(&bindings)};
		
		if (vkCreateDescriptorSetLayout(appData.device, &layoutInfo, nullptr, &appData.descriptorSetLayout) != VK_SUCCESS)
			return make_unexpected("failed to create descriptor set layout");
		
		return appData;
	}
}