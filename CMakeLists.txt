cmake_minimum_required(VERSION 3.21)
project(unrolled_linked_list)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=address")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)
