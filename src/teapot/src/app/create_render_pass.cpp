#include "app/helpers/VkObjectsHelpers.h"
#include "app/helpers/VkStructsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_render_pass(AppData data)
{
	assert(data.device);
	
	std::vector<VkAttachmentDescription> const attachments{helpers::get_attachment_description(data.surfaceFormat.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};
	std::vector<VkAttachmentReference> const colorAttachments{helpers::get_attachment_reference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)};
	std::vector<VkSubpassDescription> const subpasses{helpers::get_subpass_description(&colorAttachments)};
	
	helpers::MaybeRenderPass const mbRenderPass{helpers::create_render_pass(data.device, &subpasses, &attachments)};
	if(!mbRenderPass)
		return tl::make_unexpected(mbRenderPass.error());
	
	data.renderPass = *mbRenderPass;
	
	return std::move(data);
}

} // namespace app