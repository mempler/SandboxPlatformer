#include "Player.hh"

#include "Game/Game.hh"
#include "Game/Network/Packets/AvatarStatePacket.hh"

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

void Player::Tick( float fDelta )
{
    if ( m_pAvatar )
    {
        auto &velocity = m_pAvatar->m_v2TargetVelocity;

        if ( ( velocity.x != 0.f || velocity.y != 0.f )
             && m_MovementTimer.elapsed() >= .2f )
        {
            m_MovementTimer.reset();

            Packets::AvatarStateData data;
            data.SetAvatar( m_pAvatar );

            Packets::REQ_AvatarState packet {};
            packet.m_Object = &data;

            auto size = GetGame()->GetClient()->Send( packet );

#if ENGINE_DEBUG
            GetGame()->GetNetInspector()->HookSendPacket( packet.m_Header.m_eType, size );
#endif
        }
    }
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

    Packets::AvatarStateData data;
    data.SetAvatar( m_pAvatar );

    Packets::REQ_AvatarState packet {};
    packet.m_Object = &data;

    auto size = GetGame()->GetClient()->Send( packet );

#if ENGINE_DEBUG
    GetGame()->GetNetInspector()->HookSendPacket( packet.m_Header.m_eType, size );
#endif
}

Avatar *Player::GetAvatar()
{
    return m_pAvatar;
}