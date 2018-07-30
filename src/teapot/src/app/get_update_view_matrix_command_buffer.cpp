#include "app/App.h"

#include <cassert>

namespace app
{

MaybeCommandBuffer get_update_view_matrix_command_buffer(VkDevice const device, VkCommandPool const commandPool, VkBuffer const buffer, uint32_t const resourceIndex, char * const memPtr)
{
	assert(device);
	assert(commandPool);
	assert(buffer);
	
	update_view_matrix(memPtr);
	
	app::MaybeCommandBuffer const mbCommandBuffer{app::allocate_synchronization_buffer(device, commandPool, buffer, sizeof(float) * 16, sizeof(float) * 16 * resourceIndex)};
	if(!mbCommandBuffer)
		return tl::make_unexpected(mbCommandBuffer.error());
	
	static std::vector<VkCommandBuffer> commandBuffers{};
	
	if(commandBuffers.size() > resourceIndex)
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffers[resourceIndex]);
	else
		commandBuffers.resize(resourceIndex + 1);
	
	commandBuffers[resourceIndex] = *mbCommandBuffer;
	
	return commandBuffers[resourceIndex];
}

} // namespace app