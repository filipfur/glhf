add_executable(hello_triangle hello_triangle.cpp)
target_link_libraries(hello_triangle PRIVATE glhf)

add_executable(hello_image hello_image.cpp)
target_link_libraries(hello_image PRIVATE glhf glhf_assets)

add_executable(hello_cube hello_cube.cpp)
target_link_libraries(hello_cube PRIVATE glhf glhf_assets)

add_executable(hello_dancer hello_dancer.cpp)
target_link_libraries(hello_dancer PRIVATE glhf glhf_assets)

if(UNIX)
    target_compile_options(hello_triangle PRIVATE -Wall -Wextra)
    target_compile_options(hello_cube PRIVATE -Wall -Wextra)
    target_compile_options(hello_dancer PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(hello_triangle PRIVATE /Zc:preprocessor /W2)
    target_compile_options(hello_cube PRIVATE /Zc:preprocessor /W2)
    target_compile_options(hello_dancer PRIVATE /Zc:preprocessor /W2)
else()
    message(FATAL_ERROR "Unknown target")
endif()