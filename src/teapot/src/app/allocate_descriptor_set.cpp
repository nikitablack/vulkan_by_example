#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData allocate_descriptor_set(AppData data)
{
	assert(data.descriptorSetLayout);
	assert(data.device);
	
	std::vector<VkDescriptorSetLayout> const setLayouts{data.descriptorSetLayout};
	
	helpers::MaybeDescriptorSets const mbDescriptorSets{helpers::allocate_descriptor_sets(data.device, data.descriptorPool, &setLayouts)};
	if(!mbDescriptorSets)
		return tl::make_unexpected(mbDescriptorSets.error());
		
	data.descriptorSet = (*mbDescriptorSets)[0];
	
	return data;
}

} // namespace app