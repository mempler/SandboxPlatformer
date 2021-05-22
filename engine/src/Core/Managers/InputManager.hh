#pragma once

#include <unordered_map>

#include "InputHelper.hh"

#include <glm/glm.hpp>

#include <signals.hpp>

class InputManager
{
  public:
    void Init();

    inline bool IsKeyDown( Key eKey )
    {
        return m_vKeyState.at( (int) eKey );
    }
    inline bool IsKeyUp( Key eKey )
    {
        return !m_vKeyState.at( (int) eKey );
    }

    inline bool IsKeyModActive( KeyMod eMod )
    {
        return m_eKeyMods & eMod;
    }

    inline bool IsMouseBtnDown( MouseButton eBtn )
    {
        return m_eMouseButtonState & eBtn;
    }
    inline bool IsMouseBtnUp( MouseButton eBtn )
    {
        return m_eMouseButtonState & eBtn;
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

    // This is when writing made just right for text boxes
    signals::signal<void( uint32_t uChar, KeyMod eMod )> OnCharInput;

  private:
    std::array<bool, 137> m_vKeyState;

    glm::vec2 m_v2MouseScrollAxis;
    glm::vec2 m_v2MouseMoveDelta;

    uint8_t m_eMouseButtonState;
    KeyMod m_eKeyMods;

  private:
    void OnSetKeyState( Key, ButtonState, KeyMod );
    void OnChar( uint32_t, KeyMod );
};
