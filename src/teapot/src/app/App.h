#pragma once

#include "app/AppData.h"

#include "tl/expected.hpp"

#include <string>

struct GLFWwindow;

namespace app
{

using MaybeWindow = tl::expected<GLFWwindow *, std::string>;
using MaybeAppData = tl::expected<AppData, std::string>;

void clear(AppData data);
MaybeAppData create_debug_utils_messenger(AppData data);
MaybeAppData create_descriptor_set_layout(AppData data);
MaybeAppData create_instance(AppData data);
MaybeAppData create_logical_device(AppData data);
MaybeAppData create_pipeline_layout(AppData data);
MaybeAppData create_pipelines(AppData data);
MaybeAppData create_render_pass(AppData data);
MaybeAppData create_shader_modules(AppData data);
MaybeAppData create_surface(AppData data);
MaybeWindow create_window(uint32_t width, uint32_t height, std::string const & title);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
MaybeAppData get_physical_device(AppData data);
AppData get_required_window_extensions(AppData data);
void on_key_press(GLFWwindow * window, int key, int scancode, int action, int mods);
AppData prepare_device_features(AppData data);
VkBool32 vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData, void * pUserData);

} // namespace app