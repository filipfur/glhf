add_executable(hello_triangle hello_triangle.cpp)
target_link_libraries(hello_triangle PRIVATE glhf)

add_executable(hello_image hello_image.cpp)
target_link_libraries(hello_image PRIVATE glhf glhf_images glhf_shaders)

add_executable(hello_cube hello_cube.cpp)
target_link_libraries(hello_cube PRIVATE glhf glhf_objects glhf_shaders)

add_executable(hello_dancer hello_dancer.cpp)
target_link_libraries(hello_dancer PRIVATE glhf glhf_objects glhf_shaders)

if(EMSCRIPTEN)
    set_target_properties(hello_triangle PROPERTIES SUFFIX ".html")
    set_target_properties(hello_image PROPERTIES SUFFIX ".html")
    set_target_properties(hello_cube PROPERTIES SUFFIX ".html")
    set_target_properties(hello_dancer PROPERTIES SUFFIX ".html")
elseif(UNIX)
    target_compile_options(hello_triangle PRIVATE -Wall -Wextra)
    target_compile_options(hello_image PRIVATE -Wall -Wextra)
    target_compile_options(hello_cube PRIVATE -Wall -Wextra)
    target_compile_options(hello_dancer PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(hello_triangle PRIVATE /Zc:preprocessor /W2)
    target_compile_options(hello_image PRIVATE /Zc:preprocessor /W2)
    target_compile_options(hello_cube PRIVATE /Zc:preprocessor /W2)
    target_compile_options(hello_dancer PRIVATE /Zc:preprocessor /W2)
else()
    message(FATAL_ERROR "Unknown target")
endif()

install(TARGETS hello_triangle hello_image hello_cube hello_dancer
    RUNTIME DESTINATION bin
    COMPONENT examples)