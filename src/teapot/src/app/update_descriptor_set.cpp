#include "helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

using namespace std;

namespace app
{

AppData update_descriptor_set(AppData data)
{
	assert(data.device);
	assert(data.descriptorSet);
	assert(data.patchesBuffer);
	assert(data.projMatrixBuffer);
	assert(data.viewMatrixBuffer);
	assert(data.modelMatrixBuffer);
	
	{
		vector<VkDescriptorBufferInfo> const bufferInfos{helpers::get_descriptor_buffer_info(data.patchesBuffer, sizeof(float) * (data.teapotData.patchesTransforms.size() + data.teapotData.patchesColors.size()), 0)};
		vector<VkWriteDescriptorSet> const writeSets{helpers::get_write_descriptor_set(data.descriptorSet, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &bufferInfos)};
		vkUpdateDescriptorSets(data.device, static_cast<uint32_t>(writeSets.size()), writeSets.data(), 0, nullptr);
	}
	
	{
		vector<VkDescriptorBufferInfo> const bufferInfos{helpers::get_descriptor_buffer_info(data.projMatrixBuffer, sizeof(float) * 16),
		                                                 helpers::get_descriptor_buffer_info(data.viewMatrixBuffer, sizeof(float) * 16),
		                                                 helpers::get_descriptor_buffer_info(data.modelMatrixBuffer, sizeof(float) * 16)};
		vector<VkWriteDescriptorSet> const writeSets{helpers::get_write_descriptor_set(data.descriptorSet, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfos)};
		vkUpdateDescriptorSets(data.device, static_cast<uint32_t>(writeSets.size()), writeSets.data(), 0, nullptr);
	}
	
	return data;
}

} // namespace app