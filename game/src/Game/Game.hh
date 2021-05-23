#pragma once

#include "Server/Server.hh"

#include "Core/Graphics/Font/Label.hh"
#include "Core/Graphics/Surface/Surface.hh"

#include "Game/Debug/NetworkInspector.hh"
#include "Game/Managers/ItemInfoManager.hh"
#include "Game/Player/Player.hh"
#include "Game/World/World.hh"

#include "Network/Network.hh"

// not the best place to put
constexpr uintptr_t g_uFrameBufferFlags =
    BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT
    | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

#if GAME_SERVER
    #define GAME GetServer()
#else
    #define GAME GetGame()
#endif

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

    void OnResolutionChanged( BaseSurface *pSurface, uint32_t iWidth, uint32_t iHeight );

    // Networking
    void RequestAvatars();
    void RequestWorld( std::string_view const &svName );
    void RequestItemDB();

    ConnectionState GetNetClientState()
    {
        return m_pNetworkClient->GetState();
    }

    NetClientPtr GetClient()
    {
        return m_pNetworkClient;
    }

    NetworkInspector *GetNetInspector()
    {
        return m_pNetworkInspector;
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

    void OnStateChange( NetClientPtr, ConnectionState );
    void OnPacket( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer );
};

extern Game *GetGame();