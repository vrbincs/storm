cmake_minimum_required (VERSION 2.6)
project ( supnp_testdevice )

include ( FindPkgConfig )

set ( supnp_testdevice_VERSION_MAJOR 1 )
set ( supnp_testdevice_VERSION_MINOR 0 )

include_directories ( ${supnp_SOURCE_DIR} )
 
set ( SOURCES ${SOURCES} testdevice.cpp )

add_executable( supnp_testdevice ${SOURCES} )

target_link_libraries ( supnp_testdevice supnp )
