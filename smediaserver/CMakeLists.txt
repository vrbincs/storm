cmake_minimum_required (VERSION 2.6)
project ( smediaserver )

include ( FindPkgConfig )

set ( smediaserver_VERSION_MAJOR 1 )
set ( smediaserver_VERSION_MINOR 0 )

include_directories ( ${supnp_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Werror -pedantic -O0 -DRESOURCE_PATH=${PROJECT_SOURCE_DIR}")
 
set ( SOURCES ${SOURCES} cmediaserverdelegate.cpp )

add_library( smediaserver SHARED ${SOURCES} )

target_link_libraries ( smediaserver supnp )
