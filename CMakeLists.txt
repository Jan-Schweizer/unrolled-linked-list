cmake_minimum_required(VERSION 3.21)
project(unrolled_linked_list)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=address")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/af29db7ec28d6df1c7f0f745186884091e602e07.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tester test/tester.cpp)
target_link_libraries(tester gtest_main)

include(GoogleTest)
gtest_discover_tests(tester)