#pragma once

#include "pch.hh"

#include "Avatar.hh"

class Player {
public:
    Player();
    ~Player();
    
    Avatar &GetAvatar();

private:
    Avatar m_Avatar;
};