#include "homm/render/vulkan/helpers/VkObjectsHelpers.h"
#include "homm/render/vulkan/helpers/VkStructsHelpers.h"
#include "homm/render/Render.h"

#include <cassert>

namespace homm::render::vulkan
{

MaybeRenderData create_render_pass(RenderData data)
{
	assert(data.device);
	
	std::vector<VkAttachmentDescription> const attachments{vulkan::helpers::get_attachment_description(data.surfaceFormat.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};
	std::vector<VkAttachmentReference> const colorAttachments{vulkan::helpers::get_attachment_reference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)};
	std::vector<VkSubpassDescription> const subpasses{vulkan::helpers::get_subpass_description(&colorAttachments)};
	
	helpers::MaybeRenderPass const mbRenderPass{helpers::create_render_pass(data.device, &subpasses, &attachments)};
	if(!mbRenderPass)
		return tl::make_unexpected(mbRenderPass.error());
	
	data.renderPass = *mbRenderPass;
	
	return std::move(data);
}

} // namespace homm::render::vulkan