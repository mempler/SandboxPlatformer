#include <IceSDK/Core.hh>
#include <IceSDK/Surface.hh>

class SandboxPlatformer final : public IceSDK::IBaseApp
{
  public:
    ~SandboxPlatformer() override
    {
        IBaseApp::~IBaseApp();
    }

    void Tick( float fDelta ) override
    {
    }

    void Draw( float fDelta ) override
    {
    }

  private:
};

int main()
{
    SandboxPlatformer app;

    app.Run( IceSDK::ThreadMode::Multi );

    // IceSDK::SurfaceDesc desc;
    // desc.Width = 800;
    // desc.Height = 600;
    // desc.Title = "Random Game";

    // IceSDK::ISurface *surface = IceSDK::CreateSurface( desc );

    // bool exit = false;
    // while ( !exit )
    // {
    //     for ( auto &&event : surface->Poll() )
    //     {
    //         if ( event.Type == IceSDK::SurfaceEventType::Exit )
    //         {
    //             exit = true;
    //         }
    //     }
    // }

    // DestroySurface( &surface );
    return 0;
}
