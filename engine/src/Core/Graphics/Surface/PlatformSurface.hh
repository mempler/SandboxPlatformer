#pragma once

#include "Core/Graphics/Surface/Platform/Linux/X11Surface.hh"
#include "Core/Graphics/Surface/Platform/Win32/Win32Surface.hh"

#include "Platform.hh"

#if PLATFORM_WIN32
typedef Win32Surface PlatformSurface;
#elif PLATFORM_LINUX
typedef X11Surface PlatformSurface;
#else
    #error Undefined Platform surface for this platform!
#endif
