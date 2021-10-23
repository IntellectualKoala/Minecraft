cmake_minimum_required(VERSION 3.20.0)

project(Minecraft VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Minecraft)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
    deps/stb_image/stb_image.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

find_package(Vulkan REQUIRED)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/spdlog)
add_subdirectory(deps/vma)

add_executable(Minecraft ${SRC_FILES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Minecraft PUBLIC "/std:c++17")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        target_compile_options(Minecraft PUBLIC "/std:c++17")
    else()
        target_compile_options(Minecraft PUBLIC "-std=c++17")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Minecraft PUBLIC "-std=c++17")
endif()

target_link_libraries(Minecraft PUBLIC glfw glm Vulkan::Vulkan)

target_include_directories(Minecraft PUBLIC
                          deps/glfw/include
                          deps/glm/include
                          deps/PerlinNoise
                          deps/spdlog/include
                          deps/stb_image
                          deps/vma/include)

find_program(GLSLC glslc REQUIRED)

file(GLOB_RECURSE SHADER_FILES
    res/shaders/*.vert
    res/shaders/*.frag
    res/shaders/*.comp)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/res/shaders/)

foreach(SHADER_PATH ${SHADER_FILES})
    get_filename_component(SHADER ${SHADER_PATH} NAME)
    set(SHADER_OUTPUT_PATH ${CMAKE_BINARY_DIR}/res/shaders/${SHADER}.spv)

    add_custom_command(
           OUTPUT ${SHADER_OUTPUT_PATH}
           COMMAND ${GLSLC} -o ${SHADER_OUTPUT_PATH} ${SHADER_PATH}
           DEPENDS ${SHADER_PATH}
           IMPLICIT_DEPENDS CXX ${SHADER_PATH}
           VERBATIM)

    set_source_files_properties(${SHADER_OUTPUT_PATH} PROPERTIES GENERATED TRUE)
    target_sources(Minecraft PRIVATE ${SHADER_OUTPUT_PATH})
endforeach()

add_custom_command(
            TARGET Minecraft
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/res/textures ${CMAKE_BINARY_DIR}/res/textures)

target_precompile_headers(Minecraft PRIVATE src/pch.h)
