#pragma once

#include <unordered_map>

#include "InputHelper.hh"

#include <glm/glm.hpp>

#include <signals.hpp>

class InputManager
{
  public:
    void Init();

    bool IsKeyDown( Key eKey )
    {
        return m_umKeyState.at( eKey ) == ButtonState::Pressed;
    }
    bool IsKeyUp( Key eKey )
    {
        return m_umKeyState.at( eKey ) == ButtonState::Released;
    }

    bool IsKeyModActive( KeyMod eMod )
    {
        return m_eKeyMods & eMod;
    }

    bool IsMouseBtnDown( MouseButton eBtn )
    {
        return m_umMouseButtonState.at( eBtn ) == ButtonState::Pressed;
    }
    bool IsMouseBtnUp( MouseButton eBtn )
    {
        return m_umMouseButtonState.at( eBtn ) == ButtonState::Pressed;
    }

    const glm::vec2 GetMouseScrollAxis()
    {
        return m_v2MouseScrollAxis;
    }

    const glm::vec2 GetMouseMoveDelta()
    {
        return m_v2MouseMoveDelta;
    }

    signals::signal<void( MouseButton eButton, const glm::vec2 &v2Pos )> OnMouseDown;
    signals::signal<void( MouseButton eButton, const glm::vec2 &v2Pos )> OnMouseRelease;
    signals::signal<void( const glm::vec2 &v2Pos )> OnMouseMove;
    signals::signal<void( const glm::vec2 &v2Pos )> OnMouseScroll;

    // Key when it's down and sending continuous messages, aka almost no delay
    signals::signal<void( Key eKey, KeyMod eMod )> OnKeyDown;
    // Key when it's released and sending continuous messages, aka almost no
    // delay
    signals::signal<void( Key eKey, KeyMod eMod )> OnKeyRelease;

    // This is when key is down as in text writing type, perfect for text boxes
    signals::signal<void( Key eKey, KeyMod eMod )> OnKeyDownInput;
    // This is when key is up as in text writing type, perfect for text boxes
    signals::signal<void( Key eKey, KeyMod eMod )> OnKeyReleaseInput;

  private:
    std::unordered_map<MouseButton, ButtonState> m_umMouseButtonState;
    std::unordered_map<Key, ButtonState> m_umKeyState;

    glm::vec2 m_v2MouseScrollAxis;
    glm::vec2 m_v2MouseMoveDelta;

    KeyMod m_eKeyMods;

  private:
    void OnSetKeyState( Key, ButtonState, KeyMod );
};
