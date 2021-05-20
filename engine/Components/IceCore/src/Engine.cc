#include "IceSDK/Surface.hh"

#include <IceSDK/Core.hh>
#include <IceSDK/Utils.hh>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

using namespace IceSDK;

void Engine::Init()
{
    Console::Init();

    SetFPSLimit( ThreadID::Main );
    SetFPSLimit( ThreadID::Tick );
    SetFPSLimit( ThreadID::Draw );
}

void Engine::SetFPSLimit( ThreadID eThread, uint32_t uLimit )
{
    switch ( eThread )
    {
    case ThreadID::Main: m_MainLimiter = { uLimit }; break;
    case ThreadID::Tick: m_TickLimiter = { uLimit }; break;
    case ThreadID::Draw: m_DrawLimiter = { uLimit }; break;
    }
}

void Engine::Run( ThreadMode eMode )
{
    SurfaceDesc desc;
    desc.Width = 1280;
    desc.Height = 720;
    desc.Title = "IceSDK: Engine Window";
    desc.Flags = SurfaceFlags::Centered | SurfaceFlags::Resizable;

    Console::Info( "Creating IceSurface..." );
    m_pSurface = CreateSurface( desc );

    // Initialize BGFX if single threaded
    if ( eMode == ThreadMode::Single )
    {
        InitBGFX();
    }
    else
    {
        bgfx::renderFrame();  // Set BGFX to multi threaded
    }

    if ( eMode == ThreadMode::Multi )
    {
        Console::Info( "Starting up Tick & Draw threads" );
        m_hTickThread = std::thread( &Engine::TickThreadWorker, this );
        m_hDrawThread = std::thread( &Engine::DrawThreadWorker, this );
    }

    // while ( !m_bBGFXInitialized ) continue;

    Timer timer;
    while ( !m_bExit )
    {
        timer.reset();
        double elapsed = timer.elapsed();

        if ( eMode == ThreadMode::Single )
        {
            InternalTick( elapsed );
            InternalDraw( elapsed );
        }

        for ( auto &&event : m_pSurface->Poll() )
        {
            if ( event.Type == SurfaceEventType::Exit )
            {
                m_bExit = true;
            }
        }

        bgfx::renderFrame();
        m_MainLimiter.Wait();
    }

    if ( eMode == ThreadMode::Multi )
    {
        // Wait until BGFX has shut down.
        while ( bgfx::RenderFrame::NoContext != bgfx::renderFrame() )
        {
            m_MainLimiter.Wait();
        }

        m_hTickThread.join();
        m_hDrawThread.join();
    }
    else
    {
        bgfx::shutdown();
    }
}

void Engine::InitBGFX()
{
    // Convert platform data to BGFX Data
    SurfacePlatformData surfacePlatformData = m_pSurface->GetPlatformData();
    bgfx::PlatformData platformData {};
    platformData.ndt = surfacePlatformData.ndt;
    platformData.nwh = surfacePlatformData.nwh;
    platformData.context = surfacePlatformData.context;
    platformData.backBuffer = surfacePlatformData.backBuffer;
    platformData.backBufferDS = surfacePlatformData.backBufferDS;

    bgfx::Init bgfxInit {};
    bgfxInit.type = bgfx::RendererType::Count;  // Automatically choose a renderer.
    bgfxInit.platformData = platformData;
    bgfxInit.resolution.width = m_pSurface->GetWidth();
    bgfxInit.resolution.height = m_pSurface->GetHeight();
    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    bgfxInit.limits.transientVbSize = 134217728;

    Console::Info( "Initializing BGFX..." );
    if ( !bgfx::init( bgfxInit ) )
    {
        Console::Fatal( "Failed to initialize BGFX!" );
    }
    Console::Info( "BGFX Initialized..." );
}

void Engine::InternalTick( float fDelta )
{
}

void Engine::InternalDraw( float fDelta )
{
    bgfx::frame();
}

void Engine::TickThreadWorker()
{
    Timer timer;
    while ( !m_bExit )
    {
        timer.reset();
        double elapsed = timer.elapsed();

        InternalTick( elapsed );

        m_TickLimiter.Wait();
    }
}

void Engine::DrawThreadWorker()
{
    InitBGFX();  // API Thread

    Timer timer;
    while ( !m_bExit )
    {
        timer.reset();
        double elapsed = timer.elapsed();

        InternalDraw( elapsed );

        m_DrawLimiter.Wait();
    }

    m_bBGFXShutdown = true;
    bgfx::shutdown();
}
