#pragma once

#include "app/AppData.h"

#include "tl/expected.hpp"

#include <string>

struct GLFWwindow;

namespace app
{
	using MaybeAppData = tl::expected<AppData, std::string>;
	
	GLFWwindow* create_window(uint32_t width, uint32_t height, std::string const & title);
	AppData get_required_window_extensions(AppData appData);
	MaybeAppData create_instance(AppData appData);
	MaybeAppData create_surface(AppData appData);
	MaybeAppData get_physical_device(AppData appData);
	MaybeAppData create_logical_device(AppData appData);
	MaybeAppData create_shader_modules(AppData appData);
}