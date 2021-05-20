#pragma once

#include <cstdint>
#include <queue>
#include <string>

#include "Events.hh"

#include "Desc.hh"

#include <bgfx/bgfx.h>
#include <glm/fwd.hpp>

namespace IceSDK
{

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

        virtual bgfx::PlatformData GetPlatformData() = 0;

      protected:
        std::vector<SurfaceEvent> m_vEvents {};

        virtual void PollEvents() = 0;

        SurfaceDesc m_Desc;  // lets just store it there
    };

    typedef IBaseSurface ISurface;
}  // namespace IceSDK
