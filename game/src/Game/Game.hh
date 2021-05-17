#pragma once

#include "Core/Graphics/Font/Label.hh"

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

#if !GAME_SERVER
    void OnGameResize( GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight );
#endif

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

    void OnStateChange( NetClientPtr, ConnectionState, const char * );
};

extern Game *GetGame();