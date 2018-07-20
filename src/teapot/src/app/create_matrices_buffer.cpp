#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace
{

VkDeviceSize get_buffer_size_aligned_up(VkDevice const device, VkBuffer const buffer, VkDeviceSize const alignment)
{
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
	
	return (memRequirements.size + alignment- 1) & (~(alignment - 1));
}

app::MaybeAppData create_buffers(app::AppData data)
{
	using namespace app;
	
	VkDeviceSize const bufferSize{data.numConcurrentResources * sizeof(float) * 16};
	
	helpers::MaybeBuffer const mbProjMatrixBuffer{helpers::create_buffer(data.device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)};
	if (!mbProjMatrixBuffer)
		return tl::make_unexpected(mbProjMatrixBuffer.error());
	
	data.projMatrixBuffer = *mbProjMatrixBuffer;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(data.projMatrixBuffer), "project matrix buffer");
	
	helpers::MaybeBuffer const mbViewMatrixBuffer{helpers::create_buffer(data.device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)};
	if (!mbViewMatrixBuffer)
		return tl::make_unexpected(mbViewMatrixBuffer.error());
	
	data.viewMatrixBuffer = *mbViewMatrixBuffer;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(data.viewMatrixBuffer), "view matrix buffer");
	
	helpers::MaybeBuffer const mbModelMatrixBuffer{helpers::create_buffer(data.device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)};
	if (!mbModelMatrixBuffer)
		return tl::make_unexpected(mbModelMatrixBuffer.error());
	
	data.modelMatrixBuffer = *mbModelMatrixBuffer;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(data.modelMatrixBuffer), "model matrix buffer");
	
	return data;
}

app::MaybeAppData allocate_memory(app::AppData data)
{
	using namespace app;
	
	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(data.device, data.projMatrixBuffer, &memRequirements);
	
	helpers::MaybeMemoryPropertyIndex const mbMemPropIndex{helpers::get_supported_memory_property_index(data.physicalDevice,
	                                                                                                    memRequirements.memoryTypeBits,
	                                                                                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)};
	
	if (!mbMemPropIndex)
		return tl::make_unexpected(mbMemPropIndex.error());
	
	VkDeviceSize const bufferSizeAlignedUp{get_buffer_size_aligned_up(data.device, data.projMatrixBuffer, data.physicalDeviceProperties.limits.minUniformBufferOffsetAlignment)};
	
	helpers::MaybeDeviceMemory const mbDeviceMemory{helpers::allocate_device_memory(data.device, bufferSizeAlignedUp * 3, *mbMemPropIndex)};
	if (!mbDeviceMemory)
		return tl::make_unexpected(mbDeviceMemory.error());
	
	data.matricesDeviceMemory = *mbDeviceMemory;
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_DEVICE_MEMORY, reinterpret_cast<uint64_t>(data.matricesDeviceMemory), "matrices device memory");
	
	return data;
}

app::MaybeAppData bind_buffers(app::AppData data)
{
	VkDeviceSize offset{0};
	
	if (vkBindBufferMemory(data.device, data.projMatrixBuffer, data.matricesDeviceMemory, offset) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind project matrix buffer");
	
	offset += get_buffer_size_aligned_up(data.device, data.viewMatrixBuffer, data.physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
	if (vkBindBufferMemory(data.device, data.viewMatrixBuffer, data.matricesDeviceMemory, offset) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind view matrix buffer");
	
	offset += get_buffer_size_aligned_up(data.device, data.modelMatrixBuffer, data.physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
	if (vkBindBufferMemory(data.device, data.modelMatrixBuffer, data.matricesDeviceMemory, offset) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind view matrix buffer");
	
	return data;
}

} // namespace

namespace app
{

MaybeAppData create_matrices_buffers(AppData data)
{
	assert(data.device);
	assert(data.physicalDevice);
	assert(data.numConcurrentResources > 0);
	
	MaybeAppData const mbAppData{create_buffers(std::move(data))
	                             .and_then(allocate_memory)
	                             .and_then(bind_buffers)};
	
	return mbAppData;
}

}