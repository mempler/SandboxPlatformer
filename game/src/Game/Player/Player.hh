#pragma once

#include "Avatar.hh"

#include "Core/Managers/InputManager.hh"

#include "Game/World/World.hh"

class Player
{
  public:
    Player();
    ~Player();

    // The moment when player is inside world, we call this
    // if we somehow forget to call that,
    void InitAvatar( Avatar *avatar );

    Avatar *GetAvatar();

    // Events
    void OnKeyDown( Key eKey, KeyMod eMod );
    void OnKeyRelease( Key eKey, KeyMod eMod );

  private:
    Avatar *m_pAvatar = 0;
};