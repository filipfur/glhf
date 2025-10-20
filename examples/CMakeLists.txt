add_executable(hello_triangle hello_triangle.cpp)
target_link_libraries(hello_triangle PRIVATE glhf)

add_executable(hello_cube hello_cube.cpp)
target_link_libraries(hello_cube PRIVATE glhf glhf_assets)

add_executable(hello_dancer hello_dancer.cpp)
target_link_libraries(hello_dancer PRIVATE glhf glhf_assets)