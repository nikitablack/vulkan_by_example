#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_descriptor_pool(AppData data)
{
	assert(data.device);
	
	std::vector<VkDescriptorPoolSize> const poolSizes{helpers::get_descriptor_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1),
	                                                  helpers::get_descriptor_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3)};
	
	helpers::MaybeDescriptorPool const mbDescriptorPool{helpers::create_descriptor_pool(data.device, data.numConcurrentResources, &poolSizes)};
	if(!mbDescriptorPool)
		return tl::make_unexpected(mbDescriptorPool.error());
	
	data.descriptorPool = *mbDescriptorPool;
	
	return data;
}

} // namespace app