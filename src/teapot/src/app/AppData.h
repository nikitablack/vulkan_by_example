#pragma once

#include "TeapotData.h"

struct GLFWwindow;

namespace app
{

struct AppData
{
	GLFWwindow* window{nullptr};
	bool framebufferResized{false};
	
	TeapotData teapotData{};
};

} // namespace app