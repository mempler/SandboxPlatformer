#pragma once

// Disable clang format so we can align includes like what we want
// clang-format off
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>

#include <EASTL/chrono.h>
#include <EASTL/algorithm.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/span.h>

#include "Platform.hh"
// clang-format on

// Logger
#include "Core/Utils/Logger.hh"

// OS helper
#if PLATFORM_WIN32
#define NOMINMAX
#include <Windows.h>
#endif

// Graphics helper
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/timer.h>

#if PLATFORM_WIN32
#include <SDL.h>
#include <SDL_syswm.h>
#else // Linux platform difference
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#endif

// Dumb X11
#ifdef None
#undef None
#endif
#ifdef DestroyAll
#undef DestroyAll
#endif

// Audio
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#undef main