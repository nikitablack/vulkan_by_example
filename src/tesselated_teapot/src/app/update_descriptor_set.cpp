#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	AppData update_descriptor_set(AppData appData)
	{
		assert(appData.device);
		assert(appData.descriptorSets.size() == appData.projMatricesBuffers.size());
		assert(appData.descriptorSets.size() == appData.viewMatricesBuffers.size());
		assert(appData.descriptorSets.size() == appData.modelMatricesBuffers.size());
		
		for(size_t i{0}; i < appData.descriptorSets.size(); ++i)
		{
			{
				vector<VkDescriptorBufferInfo> const bufferInfos{get_descriptor_buffer_info(appData.patchesBuffer, sizeof(float) * (appData.teapotData.patchesTransforms.size() + appData.teapotData.patchesColors.size()), 0)};
				VkWriteDescriptorSet const writeSet{get_write_descriptor_set(appData.descriptorSets[i], 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &bufferInfos)};
				vkUpdateDescriptorSets(appData.device, 1, &writeSet, 0, nullptr);
			}
			
			{
				vector<VkDescriptorBufferInfo> const bufferInfos{get_descriptor_buffer_info(appData.projMatricesBuffers[i], sizeof(float) * 16),
				                                                 get_descriptor_buffer_info(appData.viewMatricesBuffers[i], sizeof(float) * 16),
				                                                 get_descriptor_buffer_info(appData.modelMatricesBuffers[i], sizeof(float) * 16)};
				vector<VkWriteDescriptorSet> const writeSets{get_write_descriptor_set(appData.descriptorSets[i], 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfos)};
				vkUpdateDescriptorSets(appData.device, static_cast<uint32_t>(writeSets.size()), writeSets.data(), 0, nullptr);
			}
		}
		
		return appData;
	}
}