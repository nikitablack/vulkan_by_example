#include "app/App.h"

#include <cassert>

namespace app
{

AppData destroy_frame_buffers(AppData data)
{
	assert(data.device);
	
	for(VkImageView const imageView : data.swapChainImageViews)
		vkDestroyImageView(data.device, imageView, nullptr);
	
	data.swapChainImageViews.clear();
	
	for(VkFramebuffer const framebuffer : data.swapChainFramebuffers)
		vkDestroyFramebuffer(data.device, framebuffer, nullptr);
	
	data.swapChainFramebuffers.clear();
	
	return data;
}

} // namespace app