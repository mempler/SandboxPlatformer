#pragma once

#include "Kokoro/Config.hh"

// clang-format off
#define PLATFORM_WIN32          KOKORO_WIN32
#define PLATFORM_LINUX          KOKORO_LINUX
#define PLATFORM_ANDROID        KOKORO_ANDROID
#define PLATFORM_EMSCRIPTEN     KOKORO_EMSCRIPTEN
#define PLATFORM_MAC            // TODO
#define PLATFORM_IOS            // TODO

#ifdef _WIN32
    #ifdef ENGINE_BUILD_DLL
        #ifdef _ENGINE_IS_SRC
            #define ENGINE_EXPORT __declspec(dllexport)
        #else
            #define ENGINE_EXPORT __declspec(dllimport)
        #endif
    #else
        #define ENGINE_EXPORT
    #endif
#else
    // Unix Systems
    #ifdef ENGINE_BUILD_DLL
        #ifdef _ENGINE_IS_SRC
            #define ENGINE_EXPORT __attribute__((visibility("default")))
        #else
            #define ENGINE_EXPORT
        #endif
    #else
        #define ENGINE_EXPORT
    #endif
#endif

// Platform Specific
#ifdef NDEBUG
    #define ENGINE_DEBUG 1
#else
    #define ENGINE_DEBUG 0
#endif

// clang-format on
