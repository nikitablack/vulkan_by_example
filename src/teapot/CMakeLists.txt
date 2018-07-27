cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator" CACHE FILEPATH "Path to glslangValidator executable")

find_package(Vulkan REQUIRED)

add_executable(teapot "")

target_sources(teapot PRIVATE
        src/app/helpers/VkObjectsHelpers.cpp
        src/app/helpers/VkObjectsHelpers.h
        src/app/helpers/VkRaii.h
        src/app/helpers/VkStructsHelpers.cpp
        src/app/helpers/VkStructsHelpers.h

        src/app/allocate_command_buffers.cpp
        src/app/allocate_descriptor_set.cpp
        src/app/allocate_synchronization_buffer.cpp
        src/app/App.h
        src/app/AppData.h
        src/app/clear.cpp
        src/app/create_command_buffer_fences.cpp
        src/app/create_command_pools.cpp
        src/app/create_debug_utils_messenger.cpp
        src/app/create_descriptor_pool.cpp
        src/app/create_descriptor_set_layout.cpp
        src/app/create_frame_buffers.cpp
        src/app/create_index_buffer.cpp
        src/app/create_instance.cpp
        src/app/create_logical_device.cpp
        src/app/create_matrices_buffer.cpp
        src/app/create_patches_buffer.cpp
        src/app/create_pipeline_layout.cpp
        src/app/create_pipelines.cpp
        src/app/create_positions_buffer.cpp
        src/app/create_render_pass.cpp
        src/app/create_semaphores.cpp
        src/app/create_shader_modules.cpp
        src/app/create_static_buffer.cpp
        src/app/create_surface.cpp
        src/app/create_swap_chain.cpp
        src/app/create_window.cpp
        src/app/destroy_frame_buffers.cpp
        src/app/framebuffer_size_callback.cpp
        src/app/get_device_qeues.cpp
        src/app/get_physical_device.cpp
        src/app/get_required_window_extensions.cpp
        src/app/get_surface_capabilities.cpp
        src/app/get_surface_extent.cpp
        src/app/get_update_project_matrix_command_buffer.cpp
        src/app/on_key_press.cpp
        src/app/prepare_device_features.cpp
        src/app/record_solid_command_buffer.cpp
        src/app/record_wireframe_command_buffer.cpp
        src/app/resize_swap_chain.cpp
        src/app/update_descriptor_set.cpp
        src/app/update_model_matrix.cpp
        src/app/update_projection_matrix.cpp
        src/app/update_view_matrix.cpp
        src/app/vulkan_debug_callback.cpp

        src/main.cpp
        src/MainApplication.cpp
        src/MainApplication.h
        src/TeapotData.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CXXCOMPILEOPTIONS -Wall -ansi -pedantic -Wextra -Wshadow -Weffc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CXXCOMPILEOPTIONS /W4)
endif()

set_target_properties(teapot PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(teapot PRIVATE ${CXXCOMPILEOPTIONS})

target_link_libraries(teapot PRIVATE
        eigen
        glfw
        tl
        ${Vulkan_LIBRARIES}
)

target_include_directories(teapot PRIVATE src)
target_include_directories(teapot SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})

function(addCompileShadersCommand)
    cmake_parse_arguments(addCompileShadersCommand "" "TARGET_NAME;OUTPUT_DIR" "SHADERS" ${ARGN})

    if(NOT addCompileShadersCommand_TARGET_NAME)
        message(FATAL_ERROR "Provide unique target name")
    endif()

    if(NOT addCompileShadersCommand_SHADERS)
        message(FATAL_ERROR "At least one shader file name should be provided")
    endif()

    set(FILE_NAMES "")
    set(OUTPUT_PATHS "")

    foreach(shader ${addCompileShadersCommand_SHADERS})
        get_filename_component(VAR ${shader} NAME_WE)
        list(APPEND FILE_NAMES ${VAR})
        list(APPEND OUTPUT_PATHS ${addCompileShadersCommand_OUTPUT_DIR}/${VAR}.spv)

        add_custom_command(OUTPUT ${addCompileShadersCommand_OUTPUT_DIR}/${VAR}.spv
                COMMAND ${GLSL_VALIDATOR} -V ${shader} -o ${addCompileShadersCommand_OUTPUT_DIR}/${VAR}.spv
                DEPENDS ${shader}
                )
    endforeach()

    add_custom_target(${addCompileShadersCommand_TARGET_NAME}
            DEPENDS ${OUTPUT_PATHS}
            COMMENT "Compiling ${FILE_NAMES}"
            )

    add_dependencies(teapot ${addCompileShadersCommand_TARGET_NAME})
endfunction()

addCompileShadersCommand(TARGET_NAME shaders
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin
        SHADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/VertexShader.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/TesselationControlShader.tesc
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/TesselationEvaluationShader.tese
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/FragmentShader.frag
        )