#include "app/App.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData allocate_command_buffers(AppData appData)
	{
		assert(appData.device);
		assert(appData.commandPoolRender);
		assert(appData.commandPoolPushConstants);
		
		{
			MaybeCommandBuffers const mbCommandBuffer{vk_helpers::allocate_command_buffers(appData.device, appData.commandPoolRender,
			                                                                               static_cast<uint32_t>(appData.swapChainFramebuffers.size()))};
			if (!mbCommandBuffer)
				return make_unexpected(mbCommandBuffer.error());
			
			appData.commandBuffersWireframe = *mbCommandBuffer;
		}
		
		{
			MaybeCommandBuffers const mbCommandBuffer{vk_helpers::allocate_command_buffers(appData.device, appData.commandPoolRender,
			                                                                               static_cast<uint32_t>(appData.swapChainFramebuffers.size()))};
			if (!mbCommandBuffer)
				return make_unexpected(mbCommandBuffer.error());
			
			appData.commandBuffersSolid = *mbCommandBuffer;
		}
		
		{
			MaybeCommandBuffers const mbCommandBuffer{vk_helpers::allocate_command_buffers(appData.device, appData.commandPoolPushConstants,
			                                                                               static_cast<uint32_t>(appData.swapChainFramebuffers.size()))};
			if (!mbCommandBuffer)
				return make_unexpected(mbCommandBuffer.error());
			
			appData.commandBuffersPushConstants = *mbCommandBuffer;
		}
		
		return appData;
	}
}