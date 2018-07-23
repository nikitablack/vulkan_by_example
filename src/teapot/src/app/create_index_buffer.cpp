#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_index_buffer(AppData data)
{
	assert(data.device);
	assert(data.physicalDevice);
	assert(data.graphicsQueue);
	
	StaticBufferData bufferData{};
	bufferData.device = data.device;
	bufferData.physicalDevice = data.physicalDevice;
	bufferData.dataSize = sizeof(decltype(data.teapotData.patches)::value_type) * data.teapotData.patches.size();
	bufferData.data = data.teapotData.patches.data();
	bufferData.localDeviceBufferUsage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	bufferData.copyQueueFamilyIndex = data.graphicsFamilyQueueIndex;
	bufferData.copyQueue = data.graphicsQueue;
	bufferData.localDeviceBufferAccessMask = VK_ACCESS_INDEX_READ_BIT;
	bufferData.localDeviceBufferStageMask = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
	
	MaybeStaticBufferData mbBufferData{create_static_buffer(std::move(bufferData))};
	
	if(!mbBufferData)
		return tl::make_unexpected(mbBufferData.error());
	
	bufferData = std::move(*mbBufferData);
	
	data.indexBuffer = bufferData.bufferRaii.release();
	data.indexBufferDeviceMemory = bufferData.deviceMemoryRaii.release();
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(data.indexBuffer), "index buffer");
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_DEVICE_MEMORY, reinterpret_cast<uint64_t>(data.indexBufferDeviceMemory), "index buffer device memory");
	
	return data;
}

} // namespace app