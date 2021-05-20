#pragma once

#include <atomic>
#include <thread>

#include <IceSDK/Surface.hh>
#include <IceSDK/Utils/FPSLimiter.hh>

namespace IceSDK
{
    enum class ThreadMode
    {
        Single,
        Multi
    };

    enum class ThreadID
    {
        Main,
        Tick,
        Draw,
    };

    class Engine
    {
      public:
        void Init();

        void Run( ThreadMode eMode );

        void SetFPSLimit( ThreadID eThread, uint32_t uLimit = 60 );

      private:
        void InitBGFX();

        void InternalTick( float fDelta );
        void InternalDraw( float fDelta );

        void TickThreadWorker();
        void DrawThreadWorker();

        FrameLimiter m_MainLimiter { 60 };
        FrameLimiter m_TickLimiter { 60 };
        FrameLimiter m_DrawLimiter { 60 };

        std::thread m_hTickThread {};
        std::thread m_hDrawThread {};

        std::atomic_bool m_bBGFXShutdown = false;
        std::atomic_bool m_bExit = false;
        ISurface *m_pSurface = nullptr;
    };
}  // namespace IceSDK
