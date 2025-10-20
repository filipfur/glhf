cmake_minimum_required(VERSION 3.14)
project(glhf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(main PRIVATE -Wall -Wextra)
endif()
if ( MSVC )
    target_compile_options(main PRIVATE /W4)
endif()

add_subdirectory(third_party)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

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
target_link_libraries(glhf PUBLIC glm stb ${SDL2_LIBRARY} GLEW::GLEW)

add_subdirectory(tools/embed)
add_subdirectory(assets)

add_compile_definitions(LOG_LEVEL=3)
add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)