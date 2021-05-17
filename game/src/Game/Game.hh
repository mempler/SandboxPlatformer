#pragma once

#include "Core/Graphics/Font/Label.hh"

#include "Game/Debug/NetworkInspector.hh"
#include "Game/Managers/ItemInfoManager.hh"
#include "Game/Network/Network.hh"
#include "Game/Player/Player.hh"
#include "Game/World/World.hh"

#include <steam/steamnetworkingsockets.h>

// not the best place to put
constexpr uintptr_t g_uFrameBufferFlags =
    BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT
    | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

class Game
{
  public:
    Game();
    ~Game();

    ItemInfoManager &GetItemInfoMan();

    World &GetWorld();
    Player &GetLocalPlayer();

    void Init();
    void Tick( float fDeltaTime );
    void Draw();

    void OnGameResize( GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight );

    // Networking
    void RequestWorld( std::string_view const &svName );
    void RequestItemDB();

    ConnectionState GetNetClientState()
    {
        return m_pNetworkClient->GetState();
    }

  private:
    // Item utilites
    ItemInfoManager m_ItemInfoManager;

    // Main handlers
    World m_World;
    Player m_Player;

    // Networking
    Font *m_pFont;
    Label m_lConnectionStatus;

    Network m_Network;
    NetClientPtr m_pNetworkClient;

#if ENGINE_DEBUG
    NetworkInspector *m_pNetworkInspector;
#endif

    void OnStateChange( NetClientPtr, ConnectionState, const char * );
    void OnPacket( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer );
};

extern Game *GetGame();