#pragma once

#include "Packet.hh"

#include "NetClient.hh"

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

class NetListener
{
  public:
    NetListener()
    {
    }

    NetListener( ISteamNetworkingSockets *pInstance, HSteamListenSocket hSocket ) :
        m_pInstance( pInstance ), m_hConn( hSocket )
    {
    }

    void Broadcast( IBasePacket *pPacket )
    {
    }

    bool AcceptConnection( NetClientPtr pClient )
    {
        return m_pInstance->AcceptConnection( pClient->Handle() ) != k_EResultOK;
    }

  private:
    HSteamListenSocket m_hConn = 0;
    ISteamNetworkingSockets *m_pInstance = nullptr;
};