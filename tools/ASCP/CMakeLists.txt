set(ASCP_LIBS
    bx
    bgfx
    bimg

    Kokoro::Memory
    Kokoro::FileSystem
    Kokoro::Utility
    lw
    zlib
)

file(GLOB_RECURSE ASCP_SOURCES 
    src/*.cc
)

add_executable(ASCP ${ASCP_SOURCES})
    target_precompile_headers(ASCP PUBLIC src/pch.hh)
    target_link_libraries(ASCP PUBLIC ${ASCP_LIBS})
    target_include_directories(ASCP PUBLIC src/)
