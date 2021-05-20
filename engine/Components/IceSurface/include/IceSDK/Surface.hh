#pragma once

#include "Surface/Desc.hh"
#include "Surface/Events.hh"
#include "Surface/ISurface.hh"
#include "Surface/KeyTable.hh"

// Cross platform windowing/surface implementation
// and specific OS/platform abstraction tool and utility
// damn, feel like doing professional stuff now. hire me valve

namespace IceSDK
{
    ISurface *CreateSurface( const SurfaceDesc &desc );
    void DestroySurface( ISurface **ppSurface );
}  // namespace IceSDK
