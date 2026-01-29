include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(glhf_test_json
    src/test_json.cpp
)
target_link_libraries(glhf_test_json GTest::gtest_main glhf)

add_executable(glhf_test_gltf
    src/test_gltf.cpp
)
target_link_libraries(glhf_test_gltf GTest::gtest_main glhf glhf_objects)

add_executable(glhf_test_geometry
    src/test_geometry.cpp
)
target_link_libraries(glhf_test_geometry GTest::gtest_main glhf)

add_executable(glhf_test_trs
    src/test_trs.cpp
)
target_link_libraries(glhf_test_trs GTest::gtest_main glhf)

include(GoogleTest)
gtest_discover_tests(glhf_test_json)
gtest_discover_tests(glhf_test_gltf)
gtest_discover_tests(glhf_test_geometry)