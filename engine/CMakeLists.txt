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
    lw
    SDL2-static
    zlib
    OpenAL
    bgfx-imgui
    imgui
    implot
    freetype-bgfx
    fmt
)

file(GLOB_RECURSE ENGINE_SOURCES 
    src/*.cc
)

add_library(${PROJECT_NAME}Engine STATIC ${ENGINE_SOURCES}  ${CMAKE_CURRENT_BINARY_DIR}/embedded_resources.cc)
    target_precompile_headers(${PROJECT_NAME}Engine PUBLIC src/pch.hh)
    target_link_libraries(${PROJECT_NAME}Engine PUBLIC ${ENGINE_LIBS})
    target_include_directories(${PROJECT_NAME}Engine PUBLIC src/)
    

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/embedded_resources.cc
    COMMAND python ${CMAKE_SOURCE_DIR}/engine/generate_embedded.py
)
