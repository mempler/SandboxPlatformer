##############################
# Variables

set(ComponentName IceSurface)

set(RequiredComponents
    glm
    bgfx

    Kokoro::Utility
    Kokoro::Config
)

##############################
# Internals

file(GLOB_RECURSE SOURCES 
    src/*.cc
)

add_library(${ComponentName} STATIC ${SOURCES})
    target_link_libraries(${ComponentName} PUBLIC ${RequiredComponents})
    target_include_directories(${ComponentName} PRIVATE src/ PUBLIC include/)
