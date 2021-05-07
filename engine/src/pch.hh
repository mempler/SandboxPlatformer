#pragma once

// Disable clang format so we can align includes like what we want
// clang-format off
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <cstddef>
#include <iterator>
#include <unordered_map>
#include <string_view>
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <type_traits>
#include <utility>

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

// Kokoro
#include <Kokoro/FileSystem.hh>

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
#include <signals.hpp>
#include <span.hpp>

// Our own stuff
// this is stuff that we wont change at all (or super rarely)
#include "Core/Audio/Helper.hh"
#include "Core/Managers/IResourceManager.hh"
#include "Core/Utils/FileSystem.hh"
#include "Core/Utils/Identifier.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Math.hh"
#include "Core/Utils/Memory.hh"
#include "Core/Utils/String.hh"
#include "Core/Utils/Timer.hh"
#include "Core/Utils/XorStr.hh"

#undef main