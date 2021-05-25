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

void InputManager::OnSetKeyState( Key eKey, ButtonState eState, KeyMod eMod )
{
    m_eKeyMods = eMod;
    m_vKeyState [ (int) eKey ] = (bool) eState;

    if ( eState == ButtonState::Pressed )
    {
        OnKeyDown( eKey, eMod );
    }
    else
    {
        OnKeyRelease( eKey, eMod );
    }
}

void InputManager::OnChar( uint32_t uChar, KeyMod eMod )
{
    OnCharInput( uChar, eMod );
}
