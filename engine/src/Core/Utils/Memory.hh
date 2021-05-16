#pragma once

#include <Kokoro/fmt/format.h>

namespace Memory
{
    inline std::string SizeInText( const double pLen )
    {
        if ( pLen < 1024.0 )
        {
            return fmt::format( "{:.2f}B", pLen );
        }

        if ( pLen < 1024.0 * 1024.0 )
        {
            return fmt::format( "{:.2f}KB", pLen / 1024.0 );
        }

        if ( pLen < 1024.0 * 1024.0 * 1024.0 )
        {
            return fmt::format( "{:.2f}MB", pLen / ( 1024.0 * 1024.0 ) );
        }

        if ( pLen < 1024.0 * 1024.0 * 1024.0 * 1024.0 )
        {
            return fmt::format( "{:.2f}GB",
                                pLen / ( 1024.0 * 1024.0 * 1024.0 ) );
        }

        return fmt::format( "{:.2f}B", pLen );
    }
}  // namespace Memory
