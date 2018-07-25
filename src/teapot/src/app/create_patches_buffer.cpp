#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_patches_buffer(AppData data)
{
	assert(data.device);
	assert(data.physicalDevice);
	assert(data.graphicsQueue);
	assert(data.teapotData.patchesTransforms.size() / 16 == data.teapotData.patchesColors.size() / 4);
	
	std::vector<float> patchesData{};
	patchesData.reserve(data.teapotData.patchesTransforms.size() + data.teapotData.patchesColors.size());
	
	for(size_t i{0}; i < data.teapotData.patchesTransforms.size() / 16; ++i)
	{
		patchesData.insert(patchesData.end(), data.teapotData.patchesTransforms.begin() + i * 16, data.teapotData.patchesTransforms.begin() + i * 16 + 16);
		patchesData.insert(patchesData.end(), data.teapotData.patchesColors.begin() + i * 4, data.teapotData.patchesColors.begin() + i * 4 + 4);
	}
	
	StaticBufferData bufferData{};
	bufferData.device = data.device;
	bufferData.physicalDevice = data.physicalDevice;
	bufferData.dataSize = sizeof(decltype(patchesData)::value_type) * patchesData.size();
	bufferData.data = patchesData.data();
	bufferData.localDeviceBufferUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	bufferData.copyQueueFamilyIndex = data.graphicsFamilyQueueIndex;
	bufferData.copyQueue = data.graphicsQueue;
	bufferData.localDeviceBufferAccessMask = VK_ACCESS_SHADER_READ_BIT;
	bufferData.localDeviceBufferStageMask = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
	
	MaybeStaticBufferData mbBufferData{create_static_buffer(std::move(bufferData))};
	if(!mbBufferData)
		return tl::make_unexpected(mbBufferData.error());
	
	bufferData = std::move(*mbBufferData);
	
	data.patchesBuffer = bufferData.bufferRaii.release();
	data.patchesBufferDeviceMemory= bufferData.deviceMemoryRaii.release();
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(data.patchesBuffer), "patches buffer");
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_DEVICE_MEMORY, reinterpret_cast<uint64_t>(data.patchesBufferDeviceMemory), "patches buffer device memory");
	
	return data;
}

} // namespace app