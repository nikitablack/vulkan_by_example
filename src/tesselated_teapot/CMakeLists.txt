cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator" CACHE FILEPATH "Path to glslangValidator executable")

find_package(Vulkan REQUIRED)

add_executable(tesselated_teapot "")

target_sources(tesselated_teapot PRIVATE
        src/app/allocate_command_buffers.cpp
        src/app/allocate_descriptor_sets.cpp
        src/app/App.h
        src/app/AppData.h
        src/app/clear.cpp
        src/app/create_command_pools.cpp
        src/app/create_debug_report_callback.cpp
        src/app/create_descriptor_pool.cpp
        src/app/create_descriptor_set_layout.cpp
        src/app/create_fences.cpp
        src/app/create_frame_buffers.cpp
        src/app/create_graphics_pipelines.cpp
        src/app/create_index_buffer.cpp
        src/app/create_instance.cpp
        src/app/create_logical_device.cpp
        src/app/create_matrices_buffer.cpp
        src/app/create_patches_buffer.cpp
        src/app/create_pipeline_layout.cpp
        src/app/create_positions_buffer.cpp
        src/app/create_present_semaphores.cpp
        src/app/create_render_pass.cpp
        src/app/create_shader_modules.cpp
        src/app/create_surface.cpp
        src/app/create_swap_chain.h
        src/app/create_window.cpp
        src/app/get_device_qeues.cpp
        src/app/get_physical_device.cpp
        src/app/get_required_window_extensions.cpp
        src/app/update_descriptor_set.cpp

        src/details/device_local_buffer/allocate_command_buffer.cpp
        src/details/device_local_buffer/bind_device_local_buffer_and_memory.cpp
        src/details/device_local_buffer/bind_staging_buffer_and_memory.cpp
        src/details/device_local_buffer/copy_buffer.cpp
        src/details/device_local_buffer/copy_data_to_staging_buffer.cpp
        src/details/device_local_buffer/create_copy_command_pool.cpp
        src/details/device_local_buffer/create_device_local_buffer.cpp
        src/details/device_local_buffer/create_device_local_buffer_with_data.cpp
        src/details/device_local_buffer/create_device_local_buffer_with_data.h
        src/details/device_local_buffer/create_device_local_device_memory.cpp
        src/details/device_local_buffer/create_staging_buffer.cpp
        src/details/device_local_buffer/create_staging_device_memory.cpp
        src/details/device_local_buffer/find_device_local_buffer_memory_type.cpp
        src/details/device_local_buffer/find_staging_buffer_memory_type.cpp
        src/details/vk_raii/VkRaii.h
        src/details/calculate_total_buffers_size.cpp
        src/details/check_device_suitability.cpp
        src/details/check_required_device_extensions.cpp
        src/details/create_frame_buffers.cpp
        src/details/create_image_views.cpp
        src/details/Details.h
        src/details/get_device_graphics_and_present_queue_families.cpp
        src/details/get_device_surface_format.cpp
        src/details/get_device_surface_present_mode.cpp
        src/details/get_surface_extent.cpp
        src/details/get_swap_chain_images.cpp

        src/vk_helpers/VkObjectsHelper.cpp
        src/vk_helpers/VkObjectsHelper.h
        src/vk_helpers/VkStructHelpers.cpp
        src/vk_helpers/VkStructHelpers.h

        src/Main.cpp
        src/MainApplication.cpp
        src/MainApplication.h
        src/TeapotData.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CXXCOMPILEOPTIONS -Wall -ansi -pedantic -Wextra -Wshadow -Weffc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CXXCOMPILEOPTIONS /W4)
endif()

set_target_properties(tesselated_teapot PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(tesselated_teapot PRIVATE ${CXXCOMPILEOPTIONS})

target_link_libraries(tesselated_teapot PRIVATE
        eigen
        glfw
        tl
        ${Vulkan_LIBRARIES}
)

target_include_directories(tesselated_teapot PRIVATE src)
target_include_directories(tesselated_teapot SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})

function(addCompileShaderCommand)
    cmake_parse_arguments(addCompileShaderCommand "" "SHADER" "" ${ARGN})

    if(NOT addCompileShaderCommand_SHADER)
        message(FATAL_ERROR "Shader file name should be provided")
    endif()

    add_custom_command(OUTPUT ${addCompileShaderCommand_SHADER}
            COMMAND ${GLSL_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/resources/${addCompileShaderCommand_SHADER} -o ${CMAKE_BINARY_DIR}/bin/${addCompileShaderCommand_SHADER}.spv
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources/${addCompileShaderCommand_SHADER})
endfunction()

addCompileShaderCommand(SHADER VertexShader.vert)
addCompileShaderCommand(SHADER TesselationControlShader.tesc)
addCompileShaderCommand(SHADER TesselationEvaluationShader.tese)
addCompileShaderCommand(SHADER FragmentShader.frag)

add_custom_target(shaders
        DEPENDS VertexShader.vert TesselationControlShader.tesc TesselationEvaluationShader.tese FragmentShader.frag
        COMMENT "Compiling shaders")