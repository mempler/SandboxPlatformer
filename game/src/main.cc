#include <IceSDK/Surface.hh>

int main()
{
    IceSDK::SurfaceDesc desc;
    desc.Width = 800;
    desc.Height = 600;
    desc.Title = "Random Game";

    IceSDK::ISurface *surface = IceSDK::CreateSurface( desc );

    bool exit = false;
    while ( !exit )
    {
        for ( auto &&event : surface->Poll() )
        {
            if ( event.Type == IceSDK::SurfaceEventType::Exit )
            {
                exit = true;
            }
        }
    }

    DestroySurface( &surface );
    return 0;
}
