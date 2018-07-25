#include "app/App.h"

#include "GLFW/glfw3.h"

#include <algorithm>
#include <cassert>
#include <limits>

namespace app
{

AppData get_surface_extent(AppData data)
{
	assert(data.window);
	
	int windowWidth{0};
	int windowHeight{0};
	glfwGetFramebufferSize(data.window, &windowWidth, &windowHeight);
	
	VkExtent2D swapChainExtent{};
	if (data.surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		swapChainExtent.width = std::max(data.surfaceCapabilities.minImageExtent.width, std::min(data.surfaceCapabilities.maxImageExtent.width, static_cast<uint32_t>(windowWidth)));
		swapChainExtent.height = std::max(data.surfaceCapabilities.minImageExtent.height, std::min(data.surfaceCapabilities.maxImageExtent.height, static_cast<uint32_t>(windowHeight)));
	}
	else
		swapChainExtent = data.surfaceCapabilities.currentExtent;
	
	data.surfaceExtent = swapChainExtent;
	
	return std::move(data);
}

} // namespace app