#include "GLFW/glfw3.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <vulkan/vulkan.h>

using namespace std;

namespace details
{
	VkExtent2D get_surface_extent(GLFWwindow * const window, VkSurfaceCapabilitiesKHR const & surfaceCapabilities)
	{
		if (surfaceCapabilities.currentExtent.width != numeric_limits<uint32_t>::max())
			return surfaceCapabilities.currentExtent;
		
		int windowWidth{0};
		int windowHeight{0};
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		
		return {max(surfaceCapabilities.minImageExtent.width, min(surfaceCapabilities.maxImageExtent.width, static_cast<uint32_t>(windowWidth))),
		        max(surfaceCapabilities.minImageExtent.height, min(surfaceCapabilities.maxImageExtent.height, static_cast<uint32_t>(windowHeight)))};
	}
}