cmake_minimum_required(VERSION 3.14)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    set(VCPKG_LIBRARY_LINKAGE static)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(glhf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_library(glhf
    src/animation.cpp
    src/camera.cpp
    src/collection.cpp
    src/color.cpp
    src/framebuffer.cpp
    src/gltf.cpp
    src/json.cpp
    src/mesh.cpp
    src/node.cpp
    src/shader.cpp
    src/skin.cpp
    src/time.cpp
    src/timer.cpp
    src/texture.cpp
    src/trs.cpp
    src/uniform.cpp
    src/uniformbuffer.cpp
    src/window.cpp
)
target_include_directories(glhf PUBLIC ${SDL2_INCLUDE_DIR} include)
target_link_libraries(glhf PUBLIC glm stb SDL2::SDL2-static SDL2::SDL2main GLEW::GLEW OpenGL::GL)

if(UNIX)
    target_compile_options(glhf PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(glhf PRIVATE /Zc:preprocessor /W2)
else()
    message(FATAL_ERROR "Unknown target")
endif()

add_subdirectory(tools/embed)
add_subdirectory(assets)

add_compile_definitions(LOG_LEVEL=3)
add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)