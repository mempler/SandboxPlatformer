cmake_minimum_required(VERSION 3.18.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(SandboxPlatformer VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(tools)
