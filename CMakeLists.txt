cmake_minimum_required(VERSION 3.20)
project(binary_tree_lab CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(tree_lib INTERFACE)
target_include_directories(tree_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(binary_tree_app main.cpp)
target_link_libraries(binary_tree_app PRIVATE tree_lib)

enable_testing()

add_executable(test_binary_tree tests/test_binary_tree.cpp)
target_link_libraries(test_binary_tree PRIVATE tree_lib GTest::gtest_main)

add_executable(bench_tree bench_tree.cpp)
target_link_libraries(bench_tree PRIVATE tree_lib)

add_test(NAME test_binary_tree COMMAND test_binary_tree)