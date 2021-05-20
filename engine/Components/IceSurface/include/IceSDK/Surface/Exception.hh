#pragma once

#include <string_view>

#include <Kokoro/Exception/BaseException.hh>

#define SURFACE_EXCEPTION( _FMT ) ::IceSDK::SurfaceException( _FMT, __FILE__, __LINE__ )

#define SURFACE_EXCEPTION_FMT( _FMT, ... )                                               \
    ::IceSDK::SurfaceException( fmt::format( _FMT, __VA_ARGS__ ), __FILE__, __LINE__ )

namespace IceSDK
{
    class SurfaceException final : public Kokoro::Exception::BaseException
    {
      public:
        explicit SurfaceException( std::string_view svReason, const char *svWhere,
                                   size_t sLine ) :
            BaseException( "IceSurface", svReason, svWhere, sLine )
        {
        }
    };
}  // namespace IceSDK
