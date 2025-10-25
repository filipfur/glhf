include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(glhf_test
    src/test_json.cpp
    src/test_gltf.cpp
)
target_include_directories(glhf_test PRIVATE include)
target_link_libraries(glhf_test GTest::gtest_main glhf glhf_objects)


include(GoogleTest)
gtest_discover_tests(glhf_test)