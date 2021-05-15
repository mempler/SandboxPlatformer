#include "Player.hh"

Player::Player() {
}

Player::~Player() {
}

void Player::InitAvatar(Avatar *avatar) {
    // this is really unsafe, but it points there so no problem
    m_pAvatar = avatar;
}

void Player::OnKeyDown(Key eKey, KeyMod eMod) {
    if (!m_pAvatar)
        return;

    if (eKey == Key::Key_W) {
        GetAvatar()->SetTargetVel({ GetAvatar()->GetTargetVelocity().x, -g_fVelY });
    }

    if (eKey == Key::Key_S) {
        GetAvatar()->SetTargetVel({ GetAvatar()->GetTargetVelocity().x, g_fVelY });
    }

    if (eKey == Key::Key_A) {
        GetAvatar()->SetTargetVel({ -g_fVelX, GetAvatar()->GetTargetVelocity().y });
    }

    if (eKey == Key::Key_D) {
        GetAvatar()->SetTargetVel({ g_fVelX, GetAvatar()->GetTargetVelocity().y });
    }
}

void Player::OnKeyRelease(Key eKey, KeyMod eMod) {
    if (!m_pAvatar)
        return;

    if (eKey == Key::Key_W) {
        GetAvatar()->SetTargetVel({ GetAvatar()->GetTargetVelocity().x, 0 });
    }

    if (eKey == Key::Key_S) {
        GetAvatar()->SetTargetVel({ GetAvatar()->GetTargetVelocity().x, 0 });
    }

    if (eKey == Key::Key_A) {
        GetAvatar()->SetTargetVel({ 0, GetAvatar()->GetTargetVelocity().y });
    }

    if (eKey == Key::Key_D) {
        GetAvatar()->SetTargetVel({ 0, GetAvatar()->GetTargetVelocity().y });
    }
}

Avatar *Player::GetAvatar() {
    return m_pAvatar;
}