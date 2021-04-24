###############################################################################################

option( BGFX_BUILD_TOOLS      "Build bgfx tools."                             OFF )
option( BGFX_BUILD_EXAMPLES   "Build bgfx examples."                          OFF )
option( BGFX_INSTALL          "Create installation target."                   ON  )
option( BGFX_INSTALL_EXAMPLES "Install examples and their runtimes."          OFF )
option( BGFX_CUSTOM_TARGETS   "Include convenience custom targets."           ON  )
option( BGFX_USE_OVR          "Build with OVR support."                       OFF )
option( BGFX_AMALGAMATED      "Amalgamated bgfx build for faster compilation" OFF )
option( BX_AMALGAMATED        "Amalgamated bx build for faster compilation"   OFF )
option( BGFX_CONFIG_DEBUG     "Enables debug configuration on all builds"     OFF )
option( BGFX_CONFIG_RENDERER_WEBGPU     "Enables the webgpu renderer"         OFF )

add_subdirectory(bgfx.cmake)

###############################################################################################

add_definitions(-DGLM_FORCE_PURE) # FIXME: glm crashes using SSE because of some alignment issues
add_definitions(-DGLM_ENABLE_EXPERIMENTAL) # Quaternions

add_subdirectory(glm)

###############################################################################################

add_subdirectory(Kokoro)

###############################################################################################

add_subdirectory(lw)

###############################################################################################

if (MSVC)
    if (BITS EQUAL "32")
        set(OPT_DEF_LIBC ON)
    endif()
endif()

if (UNIX)    
    find_package(SDL2 REQUIRED)

    # It's not static in reality
    # it's just a wrapper
    add_library(SDL2-static INTERFACE)
    target_link_libraries(SDL2-static INTERFACE ${SDL2_LIBRARIES})
    target_include_directories(SDL2-static INTERFACE ${SDL2_INCLUDE_DIRS})
else()
    add_subdirectory(SDL2)
endif()

###############################################################################################

add_definitions(-DZLIB_DLL)

add_subdirectory(zlib)

###############################################################################################