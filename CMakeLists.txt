cmake_minimum_required(VERSION 2.6)

project (saso-media-server)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_subdirectory(supnp)
add_subdirectory(testdevice)
add_subdirectory(testcontrol)
