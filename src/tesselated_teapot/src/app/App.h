#pragma once

#include <string>

struct GLFWwindow;

namespace app
{
	GLFWwindow *create_window(uint32_t width, uint32_t height, std::string const &title);
}