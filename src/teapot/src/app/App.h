#pragma once

#include "app/helpers/VkRaii.h"
#include "app/AppData.h"

#include "tl/expected.hpp"

#include <string>

struct GLFWwindow;

namespace app
{

struct StaticBufferData
{
	VkDevice device{VK_NULL_HANDLE};
	VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
	VkDeviceSize dataSize{};
	void* data{nullptr};
	VkBufferUsageFlags localDeviceBufferUsage{};
	VkBufferUsageFlags localDeviceBufferAccessMask{};
	VkBufferUsageFlags localDeviceBufferStageMask{};
	uint32_t copyQueueFamilyIndex{};
	VkQueue copyQueue{VK_NULL_HANDLE};
	
	helpers::VkRaii<VkBuffer> stagingBufferRaii{};
	uint32_t stagingBufferMemoryTypeIndex{};
	helpers::VkRaii<VkDeviceMemory> stagingDeviceMemoryRaii{};
	
	helpers::VkRaii<VkBuffer> bufferRaii{};
	uint32_t bufferMemoryTypeIndex{};
	helpers::VkRaii<VkDeviceMemory> deviceMemoryRaii{};
	
	helpers::VkRaii<VkCommandPool> commandPoolRaii{};
	VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
};

using MaybeWindow = tl::expected<GLFWwindow *, std::string>;
using MaybeAppData = tl::expected<AppData, std::string>;
using MaybeStaticBufferData = tl::expected<StaticBufferData, std::string>;
using MaybeCommandBuffer = tl::expected<VkCommandBuffer, std::string>;

MaybeAppData allocate_command_buffers(AppData data);
MaybeAppData allocate_descriptor_set(AppData data);
MaybeCommandBuffer allocate_synchronization_buffer(VkDevice device, VkCommandPool commandPool, VkBuffer buffer, VkDeviceSize size, VkDeviceSize offset);
void clear(AppData data);
MaybeAppData create_command_buffer_fences(AppData data);
MaybeAppData create_command_pools(AppData appData);
MaybeAppData create_debug_utils_messenger(AppData data);
MaybeAppData create_descriptor_pool(AppData data);
MaybeAppData create_descriptor_set_layout(AppData data);
MaybeAppData create_frame_buffers(AppData data);
MaybeAppData create_index_buffer(AppData data);
MaybeAppData create_instance(AppData data);
MaybeAppData create_logical_device(AppData data);
MaybeAppData create_matrices_buffers(AppData data);
MaybeAppData create_patches_buffer(AppData data);
MaybeAppData create_pipeline_layout(AppData data);
MaybeAppData create_pipelines(AppData data);
MaybeAppData create_positions_buffer(AppData data);
MaybeAppData create_render_pass(AppData data);
MaybeAppData create_semaphores(AppData data);
MaybeAppData create_shader_modules(AppData data);
MaybeStaticBufferData create_static_buffer(StaticBufferData data);
MaybeAppData create_surface(AppData data);
MaybeAppData create_swap_chain(AppData appData);
MaybeWindow create_window(uint32_t width, uint32_t height, std::string const & title);
AppData destroy_frame_buffers(AppData data);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
AppData get_device_qeues(AppData data);
MaybeAppData get_physical_device(AppData data);
AppData get_required_window_extensions(AppData data);
MaybeAppData get_surface_capabilities(AppData data);
AppData get_surface_extent(AppData data);
MaybeCommandBuffer get_update_model_matrix_command_buffer(VkDevice device, VkCommandPool commandPool, VkBuffer buffer, uint32_t resourceIndex, uint32_t n, char * memPtr);
MaybeCommandBuffer get_update_project_matrix_command_buffer(VkDevice device, VkCommandPool commandPool, VkBuffer buffer, uint32_t resourceIndex, float aspectRatio, char * memPtr);
MaybeCommandBuffer get_update_view_matrix_command_buffer(VkDevice device, VkCommandPool commandPool, VkBuffer buffer, uint32_t resourceIndex, char * memPtr);
void on_key_press(GLFWwindow * window, int key, int scancode, int action, int mods);
AppData prepare_device_features(AppData data);
MaybeAppData record_solid_command_buffer(AppData data);
MaybeAppData record_wireframe_command_buffer(AppData data);
MaybeAppData resize_swap_chain(AppData data);
AppData update_descriptor_set(AppData data);
void update_model_matrix(uint32_t n, char * memPtr);
void update_projection_matrix(float aspectRatio, char * memPtr);
void update_view_matrix(char * memPtr);
VkBool32 vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData, void * pUserData);

} // namespace app