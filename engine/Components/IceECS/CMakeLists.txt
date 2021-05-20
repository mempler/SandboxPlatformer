##############################
# Variables
#
set(ComponentName IceECS)

set(RequiredComponents
    EnTT

    Kokoro::Exception
)

if (UNIX AND NOT APPLE) # Linux
    find_package(X11 REQUIRED)

    set(RequiredComponents ${RequiredComponents} X11)
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