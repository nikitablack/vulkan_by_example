#include "app/App.h"

#include <cassert>

namespace app
{

MaybeCommandBuffer get_update_project_matrix_command_buffer(VkDevice const device, VkCommandPool const commandPool, VkBuffer const buffer, uint32_t const resourceIndex, uint32_t const numConcurrentResources, float const aspectRatio, char * const memPtr)
{
	assert(device);
	assert(commandPool);
	assert(buffer);
	
	update_projection_matrix(aspectRatio, memPtr);
	
	MaybeCommandBuffer const mbCommandBuffer{app::allocate_synchronization_buffer(device, commandPool, buffer, sizeof(float) * 16, sizeof(float) * 16 * resourceIndex)};
	if(!mbCommandBuffer)
		return tl::make_unexpected(mbCommandBuffer.error());
	
	static std::vector<VkCommandBuffer> commandBuffers(numConcurrentResources, VK_NULL_HANDLE);
	
	if(commandBuffers[resourceIndex] != VK_NULL_HANDLE)
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffers[resourceIndex]);
	
	commandBuffers[resourceIndex] = *mbCommandBuffer;
	
	return commandBuffers[resourceIndex];
}

} // namespace app