#pragma once

#include "Avatar.hh"

#include "Game/World/World.hh"

class Player {
public:
    Player();
    ~Player();

    void Init(World &cWorld);
    void InitAvatar();

    Avatar *GetAvatar();

private:
    Avatar *m_pAvatar;
};