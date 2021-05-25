#pragma once

#include "Game/Player/Player.hh"

#include "Network/NetClient.hh"

static uint32_t s_uNetID = 1;

class SPlayer
{
  public:
    SPlayer( NetClientPtr pClient );

    // Network stuff
    bool PackLogin( Kokoro::Memory::Buffer &buffer );
    bool UnpackLogin( Kokoro::Memory::Buffer &buffer );

    NetClientPtr ClientHandle();

    uint32_t m_PID = 0; // Network/peer ID

  private:
    uint32_t m_UID = 0;

    std::string m_sName = "";

    NetClientPtr m_pClient = 0;
};