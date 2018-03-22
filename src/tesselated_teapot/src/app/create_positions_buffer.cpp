#include "app/App.h"
#include "details/Details.h"

#include <cassert>

using namespace details;
using namespace std;
using namespace tl;

namespace app
{
	MaybeAppData create_positions_buffer(AppData appData)
	{
		assert(appData.device);
		assert(appData.physicalDevice);
		assert(appData.graphicsQueue);
	
		CreateDeviceLocalBufferData bufferData{};
		bufferData.device = appData.device;
		bufferData.physicalDevice = appData.physicalDevice;
		bufferData.dataSize = sizeof(decltype(appData.teapotData.points)::value_type) * appData.teapotData.points.size();
		bufferData.data = appData.teapotData.points.data();
		bufferData.localDeviceBufferUsage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferData.copyQueueFamilyIndex = appData.graphicsFamilyQueueIndex;
		bufferData.copyQueue = appData.graphicsQueue;
		bufferData.localDeviceBufferAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		bufferData.localDeviceBufferStageMask = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		
		MaybeCreateDeviceLocalBufferData mbBufferData{ create_device_local_buffer_with_data(move(bufferData)) };
		if(!mbBufferData)
			return make_unexpected(mbBufferData.error());
		
		bufferData = move(*mbBufferData);
		
		appData.positionsBuffer = bufferData.deviceLocalBufferRaii.release();
		appData.positionsBufferDeviceMemory= bufferData.deviceLocalDeviceMemoryRaii.release();
		
		return appData;
	}
}