#pragma once

#include "Engine.hh"

namespace IceSDK
{
    class IBaseApp
    {
      public:
        virtual ~IBaseApp()
        {
            if ( m_pEngine != nullptr )
            {
                delete m_pEngine;
                m_pEngine = nullptr;
            }
        }

        void Init()
        {
            m_pEngine = new Engine;
            m_pEngine->Init();
        }

        void Run( ThreadMode eMode )
        {
            if ( m_pEngine == nullptr )
            {
                Init();
            }

            m_pEngine->Run( eMode );
        }

        virtual void Tick( float fDelta ) = 0;  // NOTE: This is on the Tick thread!
        virtual void Draw( float fDelta ) = 0;  // NOTE: This is on the Draw thread!

        inline Engine *GetEnine()
        {
            return m_pEngine;
        }

      private:
        Engine *m_pEngine = nullptr;
    };
}  // namespace IceSDK
