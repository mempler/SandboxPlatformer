#pragma once

// Disable clang format so we can align includes like what we want
#include <algorithm>
#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Platform.hh"

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

// Dumb X11
#define Font X11Font
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_syswm.h>

#undef Font

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

#include <dr_wav.h>

// Math
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// imgui
#include <imgui.h>
#include <imgui_impl_bgfx.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>
#include <implot.h>

// Utils
#include <Kokoro/Memory/Span.hh>

#include <Tracy.hpp>
#include <signals.hpp>

// Our own stuff
// this is stuff that we wont change at all (or super rarely)
#include "Core/Audio/Helper.hh"
#include "Core/Managers/IResourceManager.hh"
#include "Core/Utils/FileSystem.hh"
#include "Core/Utils/Identifier.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Math.hh"
#include "Core/Utils/Memory.hh"
#include "Core/Utils/Timer.hh"

#include <Kokoro/Config.hh>
#include <Kokoro/Exception.hh>
#include <Kokoro/FileSystem.hh>
#include <Kokoro/Memory.hh>
#include <Kokoro/Utility.hh>
#include <Kokoro/fmt.hh>

//#include "Core/Utils/XorStr.hh"

#undef main
