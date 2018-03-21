#include "app/App.h"
#include "vk_helpers/VkStructHelpers.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_render_pass(AppData appData)
	{
		assert(appData.device);
	
		vector<VkAttachmentDescription> const attachments{get_attachment_description(appData.surfaceFormat.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)};
		
		vector<VkAttachmentReference> const colorAttachments{get_attachment_reference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)};
		
		vector<VkSubpassDescription> const subpasses{get_subpass_description(&colorAttachments)};
		
		VkRenderPassCreateInfo const renderPassInfo{get_render_pass_create_info(&subpasses, &attachments)};
		
		if (vkCreateRenderPass(appData.device, &renderPassInfo, nullptr, &appData.renderPass) != VK_SUCCESS)
			return make_unexpected("failed to create render pass");
		
		return appData;
	}
}