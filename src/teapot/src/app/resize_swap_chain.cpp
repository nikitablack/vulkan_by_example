#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData resize_swap_chain(AppData data)
{
	assert(data.device);
	assert(data.swapChain);
	
	if(vkDeviceWaitIdle(data.device) != VK_SUCCESS)
		throw std::runtime_error{"failed device to wait idle"};
	
	VkSwapchainKHR const oldSwapChain{data.swapChain};
	
	MaybeAppData mbGameData{MaybeAppData{get_surface_capabilities(data)}
	                        .map(get_surface_extent)
	                        .map(destroy_frame_buffers)
	                        .and_then(create_swap_chain)
	                        .and_then(create_frame_buffers)
	                        .and_then(record_wireframe_command_buffer)
	                        .and_then(record_solid_command_buffer)};
	
	if (!mbGameData)
		return tl::make_unexpected(mbGameData.error());
	
	if(!helpers::destroy_swap_chain(data.instance, data.device, oldSwapChain))
		return tl::make_unexpected("failed to destroy swap chain");
	
	return *mbGameData;
}

} // namespace app