#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

namespace details
{
	VkDeviceSize calculate_total_buffers_size(VkDevice const device, vector<VkBuffer> const & buffers)
	{
		VkDeviceSize size{0};
		
		for(size_t i{0}; i < buffers.size(); ++i)
		{
			VkMemoryRequirements memRequirements{};
			vkGetBufferMemoryRequirements(device, buffers[i], &memRequirements);
			
			if(size % memRequirements.alignment > 0)
				size = size - (size % memRequirements.alignment) + memRequirements.alignment;
			
			size += memRequirements.size;
		}
		
		return size;
	}
}