#include "Player.hh"

Player::Player()
{
}

Player::~Player()
{
}

void Player::InitAvatar( Avatar *avatar )
{
    // this is really unsafe, but it points there so no problem
    m_pAvatar = avatar;
}

void Player::OnKeyDown( Key eKey, KeyMod eMod )
{
    if ( !m_pAvatar ) return;

    if ( eKey == Key::Key_W )
    {
        m_pAvatar->m_v2TargetVelocity.y = -g_fVelY;
    }

    if ( eKey == Key::Key_S )
    {
        m_pAvatar->m_v2TargetVelocity.y = g_fVelY;
    }

    if ( eKey == Key::Key_A )
    {
        m_pAvatar->m_v2TargetVelocity.x = -g_fVelX;
    }

    if ( eKey == Key::Key_D )
    {
        m_pAvatar->m_v2TargetVelocity.x = g_fVelX;
    }
}

void Player::OnKeyRelease( Key eKey, KeyMod eMod )
{
    if ( !m_pAvatar ) return;

    if ( eKey == Key::Key_W )
    {
        m_pAvatar->m_v2TargetVelocity.y = 0;
    }

    if ( eKey == Key::Key_S )
    {
        m_pAvatar->m_v2TargetVelocity.y = 0;
    }

    if ( eKey == Key::Key_A )
    {
        m_pAvatar->m_v2TargetVelocity.x = 0;
    }

    if ( eKey == Key::Key_D )
    {
        m_pAvatar->m_v2TargetVelocity.x = 0;
    }
}

Avatar *Player::GetAvatar()
{
    return m_pAvatar;
}