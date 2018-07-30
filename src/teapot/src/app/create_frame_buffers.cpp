#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkRaii.h"
#include "app/App.h"

#include <cassert>

namespace {

struct SwapChainImagesResourcesData
{
	SwapChainImagesResourcesData(SwapChainImagesResourcesData const &) = delete;
	SwapChainImagesResourcesData & operator=(SwapChainImagesResourcesData const &) = delete;
	SwapChainImagesResourcesData(SwapChainImagesResourcesData &&) = default;
	SwapChainImagesResourcesData & operator=(SwapChainImagesResourcesData &&) = default;
	
	std::vector<VkImage> images{};
	std::vector<app::helpers::VkRaii<VkImageView>> imageViews{};
	std::vector<app::helpers::VkRaii<VkFramebuffer>> framebuffers{};
	
	VkInstance instance{VK_NULL_HANDLE};
	VkDevice device{VK_NULL_HANDLE};
	VkSwapchainKHR swapChain{VK_NULL_HANDLE};
	VkRenderPass renderPass{VK_NULL_HANDLE};
	VkSurfaceFormatKHR format{};
	uint32_t width{0};
	uint32_t height{0};
	VkImageView depthImageView{VK_NULL_HANDLE};
};

using MaybeSwapChainResources = tl::expected<SwapChainImagesResourcesData, std::string>;

MaybeSwapChainResources get_swap_chain_images(SwapChainImagesResourcesData resourcesData)
{
	assert(resourcesData.device);
	assert(resourcesData.swapChain);
	
	uint32_t imageCount{ 0 };
	if (vkGetSwapchainImagesKHR(resourcesData.device, resourcesData.swapChain, &imageCount, nullptr) != VK_SUCCESS)
		return tl::make_unexpected("failed to get swap chain images");
	
	resourcesData.images.resize(imageCount);
	if (vkGetSwapchainImagesKHR(resourcesData.device, resourcesData.swapChain, &imageCount, resourcesData.images.data()) != VK_SUCCESS)
		return tl::make_unexpected("failed to get swap chain images");
	
	return std::move(resourcesData);
}

MaybeSwapChainResources create_image_views(SwapChainImagesResourcesData resourcesData)
{
	using namespace app;
	
	assert(resourcesData.device);
	
	resourcesData.imageViews.clear();
	resourcesData.imageViews.reserve(resourcesData.images.size());
	for (size_t i{ 0 }; i < resourcesData.images.size(); ++i)
	{
		helpers::MaybeImageView const mbImageView{helpers::create_image_view(resourcesData.device, resourcesData.images[i], VK_IMAGE_VIEW_TYPE_2D, resourcesData.format.format, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1})};
		
		if(!mbImageView)
			return tl::make_unexpected(mbImageView.error());
		
		helpers::set_debug_utils_object_name(resourcesData.instance, resourcesData.device, VK_OBJECT_TYPE_IMAGE_VIEW, reinterpret_cast<uint64_t>(*mbImageView), std::string{"swapchain image view " + std::to_string(i)}.c_str());
		
		resourcesData.imageViews.push_back(helpers::VkRaii<VkImageView>{*mbImageView, helpers::get_image_view_deleter(resourcesData.device)});
	}
	
	return std::move(resourcesData);
}

MaybeSwapChainResources create_frame_buffers(SwapChainImagesResourcesData resourcesData)
{
	using namespace app;
	
	assert(resourcesData.device);
	assert(resourcesData.renderPass);
	assert(resourcesData.depthImageView);
	
	resourcesData.framebuffers.clear();
	resourcesData.framebuffers.reserve(resourcesData.images.size());
	for(size_t i{0}; i < resourcesData.images.size(); ++i)
	{
		std::vector<VkImageView> const imageViews{resourcesData.imageViews[i].get(), resourcesData.depthImageView};
		helpers::MaybeFrameBuffer const mbFramebuffer{helpers::create_framebuffer(resourcesData.device, resourcesData.renderPass, resourcesData.width, resourcesData.height, &imageViews)};
		
		if(!mbFramebuffer)
			return tl::make_unexpected(mbFramebuffer.error());
		
		helpers::set_debug_utils_object_name(resourcesData.instance, resourcesData.device, VK_OBJECT_TYPE_FRAMEBUFFER, reinterpret_cast<uint64_t>(*mbFramebuffer), std::string{"swapchain framebuffer " + std::to_string(i)}.c_str());
		
		resourcesData.framebuffers.push_back(helpers::VkRaii<VkFramebuffer >{*mbFramebuffer, helpers::get_framebuffer_deleter(resourcesData.device)});
	}
	
	return std::move(resourcesData);
}

} // namespace

namespace app
{

MaybeAppData create_frame_buffers(AppData data)
{
	assert(data.instance);
	assert(data.device);
	assert(data.swapChain);
	assert(data.renderPass);
	
	SwapChainImagesResourcesData resourcesData{};
	resourcesData.instance = data.instance;
	resourcesData.device = data.device;
	resourcesData.swapChain = data.swapChain;
	resourcesData.renderPass = data.renderPass;
	resourcesData.format = data.surfaceFormat;
	resourcesData.width = data.surfaceExtent.width;
	resourcesData.height = data.surfaceExtent.height;
	resourcesData.depthImageView = data.depthImageView;
	
	MaybeSwapChainResources mbRes{get_swap_chain_images(std::move(resourcesData))
	                              .and_then(create_image_views)
	                              .and_then(::create_frame_buffers)};
	
	if(!mbRes)
		return tl::make_unexpected(mbRes.error());
	
	data.swapChainImageViews.clear();
	data.swapChainImageViews.reserve(mbRes->imageViews.size());
	for(helpers::VkRaii<VkImageView> & imageViewRaii : mbRes->imageViews)
		data.swapChainImageViews.push_back(imageViewRaii.release());
	
	data.swapChainFramebuffers.clear();
	data.swapChainFramebuffers.reserve(mbRes->framebuffers.size());
	for(helpers::VkRaii<VkFramebuffer> & framebufferRaii : mbRes->framebuffers)
		data.swapChainFramebuffers.push_back(framebufferRaii.release());
	
	data.numConcurrentResources = static_cast<uint32_t>(data.swapChainImageViews.size());
	
	return data;
}

} // namespace app