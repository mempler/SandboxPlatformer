set(ENGINE_LIBS
    bx
    bimg
    bgfx
    glm
    Kokoro::Memory
    Kokoro::Utility
    Kokoro::FileSystem
    Kokoro::Exception
    Kokoro::Config
    Kokoro::fmt
    lw
    SDL2-static
    zlib
    OpenAL
    bgfx-imgui
    imgui
    implot
    freetype-bgfx
    tracy
    spdlog
    sse3
)

if (LINUX AND NOT ANDROID)
    find_package(X11 REQUIRED)
    
    set(ENGINE_LIBS ${ENGINE_LIBS} X11::X11 X11::Xkb X11::xkbcommon X11::Xcursor)
endif()

file(GLOB_RECURSE ENGINE_SOURCES 
    src/*.cc
)

add_library(${PROJECT_NAME}Engine STATIC ${ENGINE_SOURCES})
    target_precompile_headers(${PROJECT_NAME}Engine PUBLIC src/pch.hh)
    target_link_libraries(${PROJECT_NAME}Engine PUBLIC ${ENGINE_LIBS})
    target_include_directories(${PROJECT_NAME}Engine PUBLIC src/)

add_library(sse3 INTERFACE IMPORTED)
if(MSVC)
    target_compile_options(sse3 INTERFACE /arch:SSE3)
else()
    target_compile_options(sse3 INTERFACE -msse3)
endif()
