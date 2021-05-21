#include "pch.hh"

#include "InputManager.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/Surface/Surface.hh"

void InputManager::Init()
{
    ZoneScoped;

    BaseSurface *pSurface = GetEngine()->GetSurface();

    pSurface->OnSetKeyState.connect<&InputManager::OnSetKeyState>( this );
    pSurface->OnChar.connect<&InputManager::OnChar>( this );
}

void InputManager::OnSetKeyState( Key eKey, ButtonState eButtonState, KeyMod eKeyMod )
{
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

void InputManager::OnChar( uint32_t uChar, KeyMod eMod )
{
    OnCharInput( uChar, eMod );
}
