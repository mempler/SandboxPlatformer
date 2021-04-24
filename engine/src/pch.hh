#pragma once

// Disable clang format so we can align includes like what we want
// clang-format off
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Platform.hh"
// clang-format on

// Logger
#include "Core/Utils/Logger.hh"

// Graphics helper
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/timer.h>

#include <SDL.h>
#include <SDL_syswm.h>
#undef main