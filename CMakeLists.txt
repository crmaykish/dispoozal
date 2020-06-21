cmake_minimum_required(VERSION 3.16.3)
project(dispoozal)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)