cmake_minimum_required (VERSION 2.6)
project ( storm_testcontrol )

include ( FindPkgConfig )

set ( storm_testcontrol_VERSION_MAJOR 1 )
set ( storm_testcontrol_VERSION_MINOR 0 )

include_directories ( ${storm_SOURCE_DIR} )
 
set ( SOURCES ${SOURCES} testcontrol.cpp )

add_executable( storm_testcontrol ${SOURCES} )

target_link_libraries ( storm_testcontrol supnp )
