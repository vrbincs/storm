cmake_minimum_required (VERSION 2.6)
project ( teststorm )

include ( FindPkgConfig )

set ( teststorm_VERSION_MAJOR 1 )
set ( teststorm_VERSION_MINOR 0 )

include_directories ( ${supnp_SOURCE_DIR} )
include_directories ( ${smediaserver_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Werror -pedantic -O0 -DRESOURCE_PATH=${PROJECT_SOURCE_DIR}")

pkg_check_modules ( GTEST REQUIRED gtest )
pkg_check_modules ( GMOCK REQUIRED gmock )

find_package (Threads REQUIRED)

add_executable ( teststorm testcmediaserverdelegate.cpp )

target_link_libraries ( teststorm ${GTEST_LIBRARIES}
                                  -lgmock
                                  ${CMAKE_THREAD_LIBS_INIT}
                                  supnp
                                  smediaserver )

include_directories ( ${GTEST_INCLUDE_DIRS}
                      ${GMOCK_INCLUDE_DIRS}
                      ${UUID_INCLUDE_DIRS}
                      ${storm_SOURCE_DIR} )
