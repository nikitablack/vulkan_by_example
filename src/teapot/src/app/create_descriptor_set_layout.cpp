#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_descriptor_set_layout(AppData data)
{
	assert(data.device);

	std::vector<VkDescriptorSetLayoutBinding> const bindings{helpers::get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, 1),
	                                                         helpers::get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, 1),
	                                                         helpers::get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, 1),
	                                                         helpers::get_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, 1)};
	
	helpers::MaybeDescriptorSetLayout const mbDescSetLayout{helpers::create_descriptor_set_layout(data.device, &bindings)};
	if(!mbDescSetLayout)
		return tl::make_unexpected(mbDescSetLayout.error());
	
	data.descriptorSetLayout = *mbDescSetLayout;
	
	return data;
}

} // namespace app