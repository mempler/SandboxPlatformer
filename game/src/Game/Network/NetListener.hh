#pragma once

#include "Packet.hh"

#include "NetClient.hh"

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

#include <Tracy.hpp>

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

    template <class P>
    void Broadcast( P &packet )
    {
    }

    bool AcceptConnection( NetClientPtr pClient )
    {
        ZoneScoped;

        return m_pInstance->AcceptConnection( pClient->Handle() ) != k_EResultOK;
    }

  private:
    HSteamListenSocket m_hConn = 0;
    ISteamNetworkingSockets *m_pInstance = nullptr;
};