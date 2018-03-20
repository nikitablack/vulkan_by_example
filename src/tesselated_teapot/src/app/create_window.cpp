#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

using namespace std;

namespace app
{
	GLFWwindow* create_window(uint32_t const width, uint32_t const height, string const & title)
	{
		if (!glfwInit())
			return nullptr;
		
		if (!glfwVulkanSupported())
			return nullptr;
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		GLFWwindow * const window{glfwCreateWindow(width, height, title.data(), nullptr, nullptr)};
		
		return window;
	}
}