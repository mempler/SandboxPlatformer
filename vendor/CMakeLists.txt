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

# This will most likely fail on WIN32 so we simply add SDL2 as subdir if it did.
find_package(SDL2)

if (SDL2_FOUND)
    # It's not static in reality
    # it's just a wrapper

    add_library(SDL2-static INTERFACE)
    target_include_directories(SDL2-static INTERFACE ${SDL2_INCLUDE_DIR})

    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        message(INFO, "Using vcpkg for SDL2.")
        target_link_libraries(SDL2-static INTERFACE SDL2::SDL2main SDL2::SDL2-static)
    else()
        target_link_libraries(SDL2-static INTERFACE ${SDL2_LIBRARY})
    endif()
else()
    message(WARNING, "SDL2 not found! It'll be compiled manually.")
    
    add_subdirectory(SDL2)
endif()

###############################################################################################

# This will most likely fail on WIN32 so we simply add SDL2 as subdir if it did.
find_package(ZLIB)
if (ZLIB_FOUND)
    add_library(zlib INTERFACE)
    target_link_libraries(zlib INTERFACE ZLIB::ZLIB)
else()
    message(WARNING, "ZLIB not found! It'll be compiled manually.")

    add_definitions(-DZLIB_DLL)
    add_subdirectory(zlib)
endif()

###############################################################################################
