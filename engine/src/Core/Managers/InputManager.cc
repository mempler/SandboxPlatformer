#include "pch.hh"

#include "InputManager.hh"

#include "Core/Engine.hh"

void InputManager::Init()
{
    ZoneScoped;

    GetEngine()->GetSurface()->OnSetKeyState.connect<&InputManager::OnSetKeyState>(
        this );
}

void InputManager::OnSetKeyState( Key eKey, ButtonState eButtonState, KeyMod eKeyMod )
{
    // TODO: Add repeating keys after X amount of time
    if ( m_vKeyState [ (int) eKey ] != (bool) eButtonState )
    {
        if ( eButtonState == ButtonState::Pressed )
        {
            OnKeyDownInput( eKey, eKeyMod );
        }
        else
        {
            OnKeyReleaseInput( eKey, eKeyMod );
        }
    }

    m_eKeyMods = eKeyMod;
    m_vKeyState [ (int) eKey ] = (bool) eButtonState;

    if ( eButtonState == ButtonState::Pressed )
    {
        OnKeyDown( eKey, eKeyMod );
    }
    else
    {
        OnKeyRelease( eKey, eKeyMod );
    }
}
