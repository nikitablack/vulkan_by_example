#include "app/App.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

using namespace std;

namespace app
{

MaybeWindow create_window(uint32_t const width, uint32_t const height, string const & title)
{
	if (!glfwInit())
		return tl::make_unexpected("failed to init glfw");
	
	if (!glfwVulkanSupported())
		return tl::make_unexpected("vulkan is not supported");
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	
	GLFWwindow * const window{glfwCreateWindow(width, height, title.data(), nullptr, nullptr)};
	if(!window)
		return tl::make_unexpected("failed to create window");
	
	return window;
}

} // namespace app