#pragma once

#if PLATFORM_WIN32
    #include "Core/Graphics/Surface/Platform/Win32/Win32Surface.hh"
typedef Win32Surface PlatformSurface;
#endif