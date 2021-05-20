#pragma once

#include <cstdint>
#include <string>

#include <Kokoro/Utility/BitFlags.hh>

namespace IceSDK
{
    enum class SurfaceFlags : uint16_t
    {
        None,
        Fullscreen = 1 << 0,
        Centered = 1 << 1,  // Desktop dist. specific
        Resizable = 1 << 2
    };
    BitFlags( SurfaceFlags );

    struct SurfaceDesc
    {
        std::string Title = "IceSDK: ISurface";
        std::string Name = "IceApp";
        std::string Icon = "";

        SurfaceFlags Flags = SurfaceFlags::None;

        uint32_t Width;
        uint32_t Height;

        int32_t X;
        int32_t Y;
    };
}  // namespace IceSDK