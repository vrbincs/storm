cmake_minimum_required (VERSION 2.6)
project ( supnp_testcontrol )

include ( FindPkgConfig )

set ( supnp_testcontrol_VERSION_MAJOR 1 )
set ( supnp_testcontrol_VERSION_MINOR 0 )

include_directories ( ${supnp_SOURCE_DIR} )
 
set ( SOURCES ${SOURCES} testcontrol.cpp )

add_executable( supnp_testcontrol ${SOURCES} )

target_link_libraries ( supnp_testcontrol supnp )
