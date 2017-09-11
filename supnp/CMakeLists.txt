cmake_minimum_required (VERSION 2.6)
project ( supnp )

include ( FindPkgConfig )

set ( supnp_VERSION_MAJOR 1 )
set ( supnp_VERSION_MINOR 0 )

pkg_check_modules ( LIBUPNP REQUIRED libupnp )

set ( SOURCES ${SOURCES} cupnpdevice.cpp 
                         cupnpctrl.cpp
                         cupnpdevicedesc.cpp
                         cupnpactiondesc.cpp
                         cupnpactionargumentdesc.cpp
                         cupnpaction.cpp
                         cupnpservice.cpp )

add_library(supnp SHARED ${SOURCES})
target_link_libraries ( supnp ${LIBUPNP_LIBRARIES} )
