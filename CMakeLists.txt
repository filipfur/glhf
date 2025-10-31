cmake_minimum_required(VERSION 3.14)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    set(VCPKG_LIBRARY_LINKAGE static)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(glhf LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(IS_TOP_LEVEL_PROJECT TRUE)
else()
    set(IS_TOP_LEVEL_PROJECT FALSE)
endif()

option(BUILD_TOOLS "BUILD_TOOLS" ON)
option(BUILD_ASSETS "BUILD_ASSETS" ${IS_TOP_LEVEL_PROJECT})
option(BUILD_EXAMPLES "BUILD_EXAMPLES" ${IS_TOP_LEVEL_PROJECT})
option(BUILD_TESTS "BUILD_TESTS" ${IS_TOP_LEVEL_PROJECT})
option(BUILD_LOG_LEVEL "BUILD_LOG_LEVEL" 3)

add_subdirectory(third_party)

find_package(OpenGL REQUIRED)
if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FULL_ES3=1 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/web/shell_minimal.html")
else()
    find_package(GLEW REQUIRED)
endif()
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
target_compile_features(glhf PUBLIC cxx_std_20)
target_include_directories(glhf PUBLIC ${SDL2_INCLUDE_DIR} include)
if(EMSCRIPTEN)
    target_link_libraries(glhf PUBLIC glm stb SDL2::SDL2 OpenGL::GL)
else()
    target_link_libraries(glhf PUBLIC glm stb SDL2::SDL2-static SDL2::SDL2main GLEW::GLEW OpenGL::GL)
endif()

if(UNIX)
    target_compile_options(glhf PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(glhf PRIVATE /Zc:preprocessor /W2)
else()
    message(FATAL_ERROR "Unknown target")
endif()

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if(BUILD_ASSETS)
    add_subdirectory(assets)
endif()

target_compile_definitions(glhf PUBLIC LOG_LEVEL=${BUILD_LOG_LEVEL})

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(NOT EMSCRIPTEN AND BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()