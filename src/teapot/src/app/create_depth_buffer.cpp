#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkRaii.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace
{

struct DepthImageData
{
	VkInstance instance{VK_NULL_HANDLE};
	VkDevice device{VK_NULL_HANDLE};
	VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
	uint32_t width{};
	uint32_t height{};
	VkFormat format{};
	uint32_t transitionQueueFamilyIndex{};
	VkQueue transitionQueue{VK_NULL_HANDLE};
	
	app::helpers::VkRaii<VkImage> imageRaii{};
	uint32_t memoryTypeIndex{};
	app::helpers::VkRaii<VkDeviceMemory> deviceMemoryRaii{};
	app::helpers::VkRaii<VkImageView> imageViewRaii{};
	app::helpers::VkRaii<VkCommandPool> commandPoolRaii{};
	VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
};

using MaybeDepthImageData = tl::expected<DepthImageData, std::string>;

MaybeDepthImageData create_image(DepthImageData data)
{
	using namespace app;
	
	assert(data.device);
	
	helpers::MaybeImage const mbImage{helpers::create_image(data.device, VkExtent3D{data.width, data.height, 1}, data.format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TYPE_2D)};
	if(!mbImage)
		return tl::make_unexpected(mbImage.error());
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_IMAGE, reinterpret_cast<uint64_t>(*mbImage), "depth image");
	
	data.imageRaii = helpers::VkRaii<VkImage>{*mbImage, helpers::get_image_deleter(data.device)};
	
	return std::move(data);
}

MaybeDepthImageData find_image_memory_type(DepthImageData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.imageRaii.get());
	assert(data.physicalDevice);
	
	VkMemoryRequirements memRequirements{};
	vkGetImageMemoryRequirements(data.device, data.imageRaii.get(), &memRequirements);
	
	helpers::MaybeMemoryPropertyIndex const mbMemPropIndex{helpers::get_supported_memory_property_index(data.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};
	if (!mbMemPropIndex)
		return tl::make_unexpected(mbMemPropIndex.error());
	
	data.memoryTypeIndex = *mbMemPropIndex;
	
	return std::move(data);
}

MaybeDepthImageData allocate_device_memory(DepthImageData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.imageRaii.get());
	
	VkMemoryRequirements memRequirements{};
	vkGetImageMemoryRequirements(data.device, data.imageRaii.get(), &memRequirements);
	
	helpers::MaybeDeviceMemory const mbDeviceMemory{helpers::allocate_device_memory(data.device, memRequirements.size, data.memoryTypeIndex)};
	if (!mbDeviceMemory)
		return tl::make_unexpected(mbDeviceMemory.error());
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_DEVICE_MEMORY, reinterpret_cast<uint64_t>(*mbDeviceMemory), "depth image device memory");
	
	data.deviceMemoryRaii = helpers::VkRaii<VkDeviceMemory>{*mbDeviceMemory, helpers::get_device_memory_deleter(data.device)};
	
	return std::move(data);
}

MaybeDepthImageData bind_image_and_memory(DepthImageData data)
{
	assert(data.device);
	assert(data.imageRaii.get());
	assert(data.deviceMemoryRaii.get());
	
	if (vkBindImageMemory(data.device, data.imageRaii.get(), data.deviceMemoryRaii.get(), 0) != VK_SUCCESS)
		return tl::make_unexpected("failed to bind image memory");
	
	return std::move(data);
}

MaybeDepthImageData create_image_view(DepthImageData data)
{
	using namespace app;
	
	assert(data.instance);
	assert(data.device);
	assert(data.imageRaii.get());
	
	helpers::MaybeImageView const mbImageView{helpers::create_image_view(data.device, data.imageRaii.get(), VK_IMAGE_VIEW_TYPE_2D, data.format, {VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1})};
	
	if(!mbImageView)
		return tl::make_unexpected(mbImageView.error());
	
	helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_IMAGE_VIEW, reinterpret_cast<uint64_t>(*mbImageView), "depth image view");
	
	data.imageViewRaii = helpers::VkRaii<VkImageView>{*mbImageView, helpers::get_image_view_deleter(data.device)};
	
	return std::move(data);
}

MaybeDepthImageData create_transition_command_pool(DepthImageData data)
{
	using namespace app;
	
	assert(data.device);
	
	helpers::MaybeCommandPool const mbCommandPool{helpers::create_command_pool(data.device, data.transitionQueueFamilyIndex)};
	if (!mbCommandPool)
		return tl::make_unexpected(mbCommandPool.error());
	
	data.commandPoolRaii = helpers::VkRaii<VkCommandPool>{*mbCommandPool, helpers::get_command_pool_deleter(data.device)};
	
	return std::move(data);
}

MaybeDepthImageData allocate_transition_command_buffer(DepthImageData data)
{
	using namespace app;
	
	assert(data.device);
	assert(data.commandPoolRaii.get());
	
	helpers::MaybeCommandBuffers const mbCommandBuffers{helpers::allocate_command_buffers(data.device, data.commandPoolRaii.get(), 1)};
	if (!mbCommandBuffers)
		return tl::make_unexpected(mbCommandBuffers.error());
	
	data.commandBuffer = (*mbCommandBuffers)[0];
	
	return std::move(data);
}

MaybeDepthImageData transition_to_layout(DepthImageData data)
{
	using namespace app;
	
	assert(data.commandBuffer);
	assert(data.imageRaii.get());
	assert(data.transitionQueue);
	
	VkCommandBufferBeginInfo beginInfo{helpers::get_command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)};
	
	if(vkBeginCommandBuffer(data.commandBuffer, &beginInfo) != VK_SUCCESS)
		return tl::make_unexpected("failed to begin command buffer");
	
	VkImageMemoryBarrier const barrier{helpers::get_image_memory_barrier(data.imageRaii.get(), 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VkImageSubresourceRange{VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1})};
	vkCmdPipelineBarrier(data.commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	
	if(vkEndCommandBuffer(data.commandBuffer) != VK_SUCCESS)
		return tl::make_unexpected("failed to end command buffer");
	
	std::vector<VkCommandBuffer> commandBuffers{data.commandBuffer};
	
	VkSubmitInfo submitInfo{helpers::get_submit_info(&commandBuffers)};
	
	if(vkQueueSubmit(data.transitionQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
		return tl::make_unexpected("failed to queue submit");
	
	if(vkQueueWaitIdle(data.transitionQueue) != VK_SUCCESS)
		return tl::make_unexpected("failed to queue wait idle");
	
	return std::move(data);
}

}; // namespace

namespace app
{

MaybeAppData create_depth_buffer(AppData data)
{
	assert(data.physicalDevice);
	
	DepthImageData depthImageData{};
	depthImageData.instance = data.instance;
	depthImageData.device = data.device;
	depthImageData.physicalDevice = data.physicalDevice;
	depthImageData.width = data.surfaceExtent.width;
	depthImageData.height = data.surfaceExtent.height;
	depthImageData.format = data.depthFormat;
	depthImageData.transitionQueueFamilyIndex = data.graphicsFamilyQueueIndex;
	depthImageData.transitionQueue = data.graphicsQueue;
	
	MaybeDepthImageData mbDepthImageData{create_image(std::move(depthImageData))
	                                     .and_then(find_image_memory_type)
	                                     .and_then(allocate_device_memory)
	                                     .and_then(bind_image_and_memory)
	                                     .and_then(create_image_view)
	                                     .and_then(create_transition_command_pool)
	                                     .and_then(allocate_transition_command_buffer)
	                                     .and_then(transition_to_layout)};
	
	if(!mbDepthImageData)
		return tl::make_unexpected(mbDepthImageData.error());
	
	depthImageData = std::move(*mbDepthImageData);
	
	data.depthImage = depthImageData.imageRaii.release();
	data.depthImageView = depthImageData.imageViewRaii.release();
	data.depthImageMemory = depthImageData.deviceMemoryRaii.release();
	
	return data;
}

} // namespace app