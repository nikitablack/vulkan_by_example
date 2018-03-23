#pragma once

#include "AppData.h"

#include "tl/expected.hpp"

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
	MaybeAppData create_debug_report_callback(AppData appData);
	void clear(AppData appData);
	MaybeAppData create_render_pass(AppData appData);
	MaybeAppData create_descriptor_set_layout(AppData appData);
	MaybeAppData create_pipeline_layout(AppData appData);
	MaybeAppData create_graphics_pipelines(AppData appData);
	MaybeAppData create_frame_buffers(AppData appData);
	MaybeAppData create_matrices_buffers(AppData appData);
	AppData get_device_qeues(AppData appData);
	MaybeAppData create_positions_buffer(AppData appData);
	MaybeAppData create_index_buffer(AppData appData);
	MaybeAppData create_patches_buffer(AppData appData);
	MaybeAppData create_command_pools(AppData appData);
	MaybeAppData allocate_command_buffers(AppData appData);
	MaybeAppData create_present_semaphores(AppData appData);
	MaybeAppData create_fences(AppData appData);
	MaybeAppData create_descriptor_pool(AppData appData);
	MaybeAppData allocate_descriptor_sets(AppData appData);
	AppData update_descriptor_set(AppData appData);
}