##############################
# Variables
#
set(ComponentName IceSurface)

set(RequiredComponents
    Kokoro::fmt
    Kokoro::Utility
    Kokoro::Config
    Kokoro::Exception
)

if (UNIX AND NOT APPLE) # Linux
    find_package(X11 REQUIRED)

    set(RequiredComponents ${RequiredComponents} X11 X11::Xkb X11::Xcursor)
endif()
#
##############################
# Internals
#
file(GLOB_RECURSE SOURCES 
    src/*.cc
)

add_library(${ComponentName} STATIC ${SOURCES})
    target_link_libraries(${ComponentName} PUBLIC ${RequiredComponents})
    target_include_directories(${ComponentName} PRIVATE src/ PUBLIC include/)
#
##############################