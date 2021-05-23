#pragma once

#include "Game/Player/Player.hh"

#include "Network/NetClient.hh"


class SPlayer
{
  public:
    SPlayer( int id )
    {
    }

  private:
    int m_ID = 0;

    Player *m_CPlayer = 0;
    NetClientPtr m_pClient = 0;
};