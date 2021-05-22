#pragma once

class Engine;
class IDebugUtil
{
  public:
    IDebugUtil( Engine *pEngine ) : m_pEngine( pEngine )
    {
    }

    virtual void Draw() = 0;

    void SetShowing( bool bIsShowing )
    {
        m_bIsShowing = bIsShowing;
    }

    bool IsShowing()
    {
        return m_bIsShowing;
    }

    virtual const char* Name() = 0;

  protected:
    bool m_bIsShowing = false;

    Engine *m_pEngine;
};
