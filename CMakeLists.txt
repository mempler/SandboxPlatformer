cmake_minimum_required(VERSION 3.18.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(SandboxPlatformer VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DNOMINMAX) # Fuck you windows
if (MSVC)
    add_definitions("/MP")
endif()

option(ENGINE_TRACING_ENABLED "Enable Tracing" OFF)
if (ENGINE_TRACING_ENABLED)
    add_definitions(-DTRACY_ENABLE) # Enable tracing solution wide.
endif()

# If LTO is supported, link with it
if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT lto_supported OUTPUT error)

    if( lto_supported )
        message(STATUS "LTO enabled")
        set(INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(STATUS "LTO not supported: <${error}>")
    endif()
endif()


add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(tools)
