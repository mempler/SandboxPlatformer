cmake_minimum_required(VERSION 3.18.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(SandboxPlatformer VERSION 0.1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(tools)
