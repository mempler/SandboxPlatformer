#pragma once

#include "Game/Network/Network.hh"
#include "Game/World/World.hh"

class Server
{
  public:
    void Init();
    void Tick( float fDeltaTime );
    void Run();

  public:
    ItemInfoManager &GetItemInfoMan()
    {
        return m_ItemInfoManager;
    }

  private:
    ItemInfoManager m_ItemInfoManager;
    World m_World;

    NetListener m_Listener;
    Network m_Network;

    void OnStateChange( NetClientPtr pClient, ConnectionState eState,
                        const char *szMessage );

    void OnPacket( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer );
};

Server *GetServer();
