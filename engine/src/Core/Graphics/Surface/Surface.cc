#include "Surface.hh"

BaseSurface::BaseSurface( SurfaceDesc &desc ) : m_Desc( desc )
{
}

SurfaceHandle BaseSurface::GetHandle() const
{
    return m_Handle;
}

uint32_t BaseSurface::GetWidth()
{
    return m_Desc.ivRes.x;
}

uint32_t BaseSurface::GetHeight()
{
    return m_Desc.ivRes.y;
}

bool BaseSurface::ShouldExit()
{
    return m_bExit;
}