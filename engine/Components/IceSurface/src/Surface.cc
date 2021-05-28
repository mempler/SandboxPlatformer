#include <memory>

#include <IceSDK/Surface.hh>
#include <Platform/Linux/X11Surface.hh>
#include <Platform/Win32/Win32Surface.hh>

using namespace IceSDK;

#if KOKORO_WIN32
typedef _internal::X11Surface PlatformSurface;
#elif KOKORO_LINUX
typedef _internal::X11Surface PlatformSurface;
#else
    #error "Platform not implemented!"
#endif

std::unique_ptr<ISurface> IceSDK::CreateSurface( const IceSDK::SurfaceDesc &desc )
{
    return std::make_unique<PlatformSurface>( desc );
}
