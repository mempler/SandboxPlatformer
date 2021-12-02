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

add_definitions(-DGLM_ENABLE_EXPERIMENTAL) # Quaternions
if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	add_compile_options(-msse3)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	add_compile_options(/QxSSE3)
elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
	add_compile_options(/arch:SSE2) # VC doesn't support /arch:SSE3
endif()

add_definitions(-DGLM_FORCE_SSE3)

add_subdirectory(glm)

###############################################################################################

add_subdirectory(Kokoro)

###############################################################################################

add_subdirectory(lw)

###############################################################################################

# This will most likely fail on WIN32 so we simply add SDL2 as subdir if it did.
find_package(ZLIB)
if (ZLIB_FOUND)
    add_library(zlib INTERFACE)
    target_link_libraries(zlib INTERFACE ZLIB::ZLIB)
else()
    message(WARNING "ZLIB not found! It'll be compiled manually.")

    add_definitions(-DZLIB_DLL)
    add_subdirectory(zlib)
endif()

###############################################################################################

find_package(OpenAL)
if (OPENAL_FOUND)
    add_library(OpenAL INTERFACE)
    target_link_libraries(OpenAL INTERFACE ${OPENAL_LIBRARY})
else()
    if (ANDROID)
        set(HAVE_ARM_NEON_H OFF)
    endif()

    option(ALSOFT_DLOPEN  "Check for the dlopen API for loading optional libs"  ON)

    option(ALSOFT_WERROR  "Treat compile warnings as errors"      OFF)

    option(ALSOFT_UTILS "Build utility programs"  OFF)
    option(ALSOFT_NO_CONFIG_UTIL "Disable building the alsoft-config utility" OFF)

    option(ALSOFT_EXAMPLES  "Build example programs"  OFF)

    option(ALSOFT_INSTALL "Install main library" ON)
    option(ALSOFT_INSTALL_CONFIG "Install alsoft.conf sample configuration file" ON)
    option(ALSOFT_INSTALL_HRTF_DATA "Install HRTF data files" ON)
    option(ALSOFT_INSTALL_AMBDEC_PRESETS "Install AmbDec preset files" ON)
    option(ALSOFT_INSTALL_EXAMPLES "Install example programs (alplay, alstream, ...)" OFF)
    option(ALSOFT_INSTALL_UTILS "Install utility programs (openal-info, alsoft-config, ...)" OFF)
    option(ALSOFT_UPDATE_BUILD_VERSION "Update git build version info" ON)

    add_subdirectory(openal-soft)
endif()

###############################################################################################

add_library(imgui STATIC imgui/imgui_demo.cpp imgui/imgui_draw.cpp
                         imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
                         imgui/imgui.cpp)
    target_include_directories(imgui PUBLIC imgui/ imgui/backends/)

add_subdirectory(bgfx-imgui)
add_subdirectory(implot)

###############################################################################################

find_package(Freetype)
if (FREETYPE_FOUND)
    add_library(freetype INTERFACE)
    target_link_libraries(freetype INTERFACE ${FREETYPE_LIBRARIES})
    target_include_directories(freetype INTERFACE ${FREETYPE_INCLUDE_DIRS})
else()
    add_subdirectory(freetype)
endif()

add_subdirectory(freetype-bgfx)

###############################################################################################

add_subdirectory(tracy)

###############################################################################################

option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" ON)
add_subdirectory(spdlog)

###############################################################################################

add_subdirectory(enet)

###############################################################################################

add_subdirectory(backward)

###############################################################################################
