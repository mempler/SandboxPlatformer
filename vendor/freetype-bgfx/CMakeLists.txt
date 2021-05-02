cmake_minimum_required(VERSION 3.0.0)
project(freetype-bgfx)

find_package(freetype CONFIG REQUIRED)

file(GLOB_RECURSE source *.cpp)
add_library(freetype-bgfx STATIC
    ${source}
)

target_link_libraries(freetype-bgfx PUBLIC bgfx freetype)
target_include_directories(freetype-bgfx PUBLIC include)