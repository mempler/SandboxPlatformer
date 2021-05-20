#pragma once

#include <cstdint>

#include <IceSDK/Surface/ISurface.hh>
#include <IceSDK/Surface/KeyTable.hh>
#include <Kokoro/Config/Platform.hh>

#if !KOKORO_ANDROID
    #if KOKORO_LINUX

namespace IceSDK::_internal
{
    class X11Surface final : public IBaseSurface
    {
      public:
        X11Surface( const SurfaceDesc &desc );
        ~X11Surface();

        bgfx::PlatformData GetPlatformData() override;

      protected:
        void PollEvents() override;

      private:
        IceSDK::KeyTable::KeyMod m_eLastKeymod;

        uintptr_t m_hDisplay;
        uintptr_t m_hWindow;
        uintptr_t m_hScreen;
    };
}  // namespace IceSDK::_internal

    #endif
#endif
