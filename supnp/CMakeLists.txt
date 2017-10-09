cmake_minimum_required (VERSION 2.6)
project ( supnp )

include ( FindPkgConfig )

set ( supnp_VERSION_MAJOR 1 )
set ( supnp_VERSION_MINOR 0 )

pkg_check_modules ( LIBUPNP REQUIRED libupnp )


include_directories(${supnp_SOURCE_DIR})

set ( SOURCES ${SOURCES} cupnpdevice.cpp 
                         cupnpctrl.cpp
                         cupnpaction.cpp
                         cupnpactionarg.cpp
                         cupnpservice.cpp
                         cstring.cpp
                         cupnphelperlinux.cpp
                         cupnpwww.cpp )

add_library(supnp SHARED ${SOURCES})
target_link_libraries ( supnp ${LIBUPNP_LIBRARIES} )
