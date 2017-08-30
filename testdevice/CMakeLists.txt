cmake_minimum_required (VERSION 2.6)
project ( storm_testdevice )

include ( FindPkgConfig )

set ( storm_testdevice_VERSION_MAJOR 1 )
set ( storm_testdevice_VERSION_MINOR 0 )

include_directories ( ${storm_SOURCE_DIR} )
 
set ( SOURCES ${SOURCES} testdevice.cpp )

add_executable( storm_testdevice ${SOURCES} )

target_link_libraries ( storm_testdevice storm )
