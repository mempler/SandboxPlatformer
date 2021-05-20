#pragma once

#include <cstdint>
#include <queue>
#include <string>
#include <string_view>
#include <utility>

#include "Events.hh"

#include "Desc.hh"

namespace IceSDK
{
    struct SurfacePlatformData
    {
        void *ndt;           // Native display type (*nix specific).
        void *nwh;           // Native window handle.
        void *context;       // GL context, or D3D device.
        void *backBuffer;    // GL back-buffer, or D3D render target view.
        void *backBufferDS;  // Backbuffer depth/stencil.
    };

    class IBaseSurface
    {
      public:
        IBaseSurface( const SurfaceDesc &desc ) : m_Desc( desc )
        {
        }

        ~IBaseSurface()
        {
        }

        inline uint32_t GetWidth()
        {
            return m_Desc.Width;
        }
        inline uint32_t GetHeight()
        {
            return m_Desc.Height;
        }

        std::vector<SurfaceEvent> Poll()
        {
            PollEvents();

            std::vector<SurfaceEvent> events {};
            m_vEvents.swap( events );

            return std::move( events );
        }

        virtual void SetTitle( const std::string_view &svName ) = 0;
        virtual void SetPosition( const int32_t X, const int32_t Y ) = 0;
        virtual void SetResolution( const uint32_t uWidth, const uint32_t uHeight ) = 0;

        virtual std::pair<uint32_t, uint32_t> GetMonitorResolution() = 0;

        virtual SurfacePlatformData GetPlatformData() = 0;

      protected:
        std::vector<SurfaceEvent> m_vEvents {};

        virtual void PollEvents() = 0;

        SurfaceDesc m_Desc;  // lets just store it there
    };

    typedef IBaseSurface ISurface;
}  // namespace IceSDK
