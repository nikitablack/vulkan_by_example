#pragma once

#include "app/AppData.h"

#include "tl/expected.hpp"

#include <string>

struct GLFWwindow;

namespace app
{

using MaybeWindow = tl::expected<GLFWwindow *, std::string>;
using MaybeAppData = tl::expected<AppData, std::string>;

MaybeAppData create_instance(AppData appData);
MaybeWindow create_window(uint32_t width, uint32_t height, std::string const & title);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void on_key_press(GLFWwindow * window, int key, int scancode, int action, int mods);

} // namespace app