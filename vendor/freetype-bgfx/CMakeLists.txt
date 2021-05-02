cmake_minimum_required(VERSION 3.0.0)
project(freetype-bgfx)

file(GLOB_RECURSE source *.cpp)
add_library(freetype-bgfx STATIC
    ${source}
)

target_link_libraries(freetype-bgfx PRIVATE bgfx)

target_include_directories(freetype-bgfx PUBLIC include)