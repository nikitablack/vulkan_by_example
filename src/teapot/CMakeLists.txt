cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator" CACHE FILEPATH "Path to glslangValidator executable")

find_package(Vulkan REQUIRED)

add_executable(tesselated_teapot "")

target_sources(tesselated_teapot PRIVATE
        src/app/helpers/VkObjectsHelpers.cpp
        src/app/helpers/VkObjectsHelpers.h
        src/app/helpers/VkStructsHelpers.cpp
        src/app/helpers/VkStructsHelpers.h

        src/app/App.h
        src/app/AppData.h
        src/app/create_instance.cpp
        src/app/create_logical_device.cpp
        src/app/create_shader_modules.cpp
        src/app/create_surface.cpp
        src/app/create_window.cpp
        src/app/framebuffer_size_callback.cpp
        src/app/get_physical_device.cpp
        src/app/get_required_window_extensions.cpp
        src/app/on_key_press.cpp

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