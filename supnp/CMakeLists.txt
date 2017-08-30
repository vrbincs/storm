cmake_minimum_required (VERSION 2.6)
project ( storm )

include ( FindPkgConfig )

set ( storm_VERSION_MAJOR 1 )
set ( storm_VERSION_MINOR 0 )

pkg_check_modules ( LIBUPNP REQUIRED libupnp )

set ( SOURCES ${SOURCES} cupnpdevice.cpp 
                         cupnpctrl.cpp )

add_library(storm SHARED ${SOURCES})

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

target_link_libraries ( storm ${LIBUPNP_LIBRARIES} )
