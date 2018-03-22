#include "app/App.h"
#include "details/Details.h"

#include <cassert>

using namespace details;
using namespace std;
using namespace tl;

namespace app
{
	MaybeAppData create_patches_buffer(AppData appData)
	{
		assert(appData.device);
		assert(appData.physicalDevice);
		assert(appData.graphicsQueue);
		assert(appData.teapotData.patchesTransforms.size() / 16 == appData.teapotData.patchesColors.size() / 4);
		
		vector<float> patchesData{};
		patchesData.reserve(appData.teapotData.patchesTransforms.size() + appData.teapotData.patchesColors.size());
		
		for(size_t i{0}; i < appData.teapotData.patchesTransforms.size() / 16; ++i)
		{
			patchesData.insert(patchesData.end(), appData.teapotData.patchesTransforms.begin() + i * 16, appData.teapotData.patchesTransforms.begin() + i * 16 + 16);
			patchesData.insert(patchesData.end(), appData.teapotData.patchesColors.begin() + i * 4, appData.teapotData.patchesColors.begin() + i * 4 + 4);
		}
		
		CreateDeviceLocalBufferData bufferData{};
		bufferData.device = appData.device;
		bufferData.physicalDevice = appData.physicalDevice;
		bufferData.dataSize = sizeof(decltype(patchesData)::value_type) * patchesData.size();
		bufferData.data = patchesData.data();
		bufferData.localDeviceBufferUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		bufferData.copyQueueFamilyIndex = appData.graphicsFamilyQueueIndex;
		bufferData.copyQueue = appData.graphicsQueue;
		bufferData.localDeviceBufferAccessMask = VK_ACCESS_SHADER_READ_BIT;
		bufferData.localDeviceBufferStageMask = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		
		MaybeCreateDeviceLocalBufferData mbBufferData{ create_device_local_buffer_with_data(move(bufferData)) };
		if(!mbBufferData)
			return make_unexpected(mbBufferData.error());
		
		bufferData = move(*mbBufferData);
		
		appData.patchesBuffer = bufferData.deviceLocalBufferRaii.release();
		appData.patchesBufferDeviceMemory= bufferData.deviceLocalDeviceMemoryRaii.release();
		
		return appData;
	}
}