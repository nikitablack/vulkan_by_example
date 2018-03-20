#pragma once

#include "TeapotData.h"

#include <string>

struct GLFWwindow;

namespace app
{
	struct AppData
	{
		GLFWwindow* window{nullptr};
		
		TeapotData teapotData{};
		float tessLevel{1.0f};
	};
}