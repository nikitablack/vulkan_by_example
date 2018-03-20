#pragma once

#include "TeapotData.h"

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