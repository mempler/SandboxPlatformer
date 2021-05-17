#pragma once

#include "Packet.hh"

#include "BaseClient.hh"

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

class BaseServer
{
  public:
    BaseServer()
    {
    }

    BaseServer( ISteamNetworkingSockets *pInstance, HSteamListenSocket hSocket ) :
        m_pInstance( pInstance ), m_hConn( hSocket )
    {
    }

    void Broadcast( IBasePacket *pPacket )
    {
    }

    bool AcceptConnection( BaseClientPtr pClient )
    {
        return m_pInstance->AcceptConnection( pClient->Handle() ) != k_EResultOK;
    }

  private:
    HSteamListenSocket m_hConn = 0;
    ISteamNetworkingSockets *m_pInstance = nullptr;
};