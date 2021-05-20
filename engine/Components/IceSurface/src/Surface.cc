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

ISurface *IceSDK::CreateSurface( const IceSDK::SurfaceDesc &desc )
{
    PlatformSurface *pSurface = new PlatformSurface( desc );

    return pSurface;
}

void IceSDK::DestroySurface( ISurface **ppSurface )
{
    if ( ppSurface == nullptr ) return;
    if ( *ppSurface == nullptr ) return;

    PlatformSurface *pSurface = static_cast<PlatformSurface *>( *ppSurface );
    delete pSurface;

    *ppSurface = nullptr;
}
