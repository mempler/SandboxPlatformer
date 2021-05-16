#include "pch.hh"

#include "InputManager.hh"

#include "Core/Engine.hh"

#include <SDL_keycode.h>

void InputManager::Init()
{
    ZoneScoped;

    GetEngine()->GetWindow().OnSDL2Event.connect<&InputManager::PumpSDL2Event>( this );

    GetEngine()->GetWindow().OnUpdate.connect<&InputManager::UpdateKeyboardState>( this );
}

void InputManager::PumpSDL2Event( GameWindow *pWindow, SDL_Event &event )
{
    ZoneScoped;

    if ( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP )
    {
        auto keyEvent = event.key.keysym;

        uint8_t keyMod = KeyMod::None;

        if ( ( keyEvent.mod & KMOD_LSHIFT ) > 0 )
        {
            keyMod |= KeyMod::SHIFT;
        }

        if ( ( keyEvent.mod & KMOD_RSHIFT ) > 0 )
        {
            keyMod |= KeyMod::SHIFT;
        }

        if ( ( keyEvent.mod & KMOD_LCTRL ) > 0 )
        {
            keyMod |= KeyMod::CONTROL;
        }

        if ( ( keyEvent.mod & KMOD_RCTRL ) > 0 )
        {
            keyMod |= KeyMod::CONTROL;
        }

        if ( ( keyEvent.mod & KMOD_RALT ) > 0 )
        {
            keyMod |= KeyMod::ALT;
        }

        if ( ( keyEvent.mod & KMOD_LALT ) > 0 )
        {
            keyMod |= KeyMod::ALT;
        }

        if ( ( keyEvent.mod & KMOD_LGUI ) > 0 )
        {
            keyMod |= KeyMod::SUPER;
        }

        if ( ( keyEvent.mod & KMOD_RGUI ) > 0 )
        {
            keyMod |= KeyMod::SUPER;
        }

        if ( ( keyEvent.mod & KMOD_NUM ) > 0 )
        {
            keyMod |= KeyMod::NUM_LOCK;
        }

        if ( ( keyEvent.mod & KMOD_CAPS ) > 0 )
        {
            keyMod |= KeyMod::CAPS_LOCK;
        }

        m_iKeyMods = (KeyMod) keyMod;
    }

    if ( event.type == SDL_KEYDOWN )
    {
        auto keyEvent = event.key.keysym.sym;
        keyEvent -= 32;

        m_umKeyState.insert_or_assign( (Key) ( keyEvent ), ButtonState::Pressed );

        OnKeyDownInput( (Key) ( keyEvent ), (KeyMod) m_iKeyMods );
    }

    if ( event.type == SDL_KEYUP )
    {
        auto keyEvent = event.key.keysym.sym;
        keyEvent -= 32;

        m_umKeyState.insert_or_assign( (Key) ( keyEvent ), ButtonState::Released );

        OnKeyReleaseInput( (Key) ( keyEvent ), (KeyMod) m_iKeyMods );
    }

    if ( event.type == SDL_MOUSEMOTION )
    {
        auto motionEvent = event.motion;

        m_v2MouseMoveDelta.x = motionEvent.x;
        m_v2MouseMoveDelta.y = motionEvent.y;

        OnMouseMove( m_v2MouseMoveDelta );
    }

    if ( event.type == SDL_MOUSEWHEEL )
    {
        auto scrollEvent = event.wheel;

        m_v2MouseScrollAxis.x = scrollEvent.x;
        m_v2MouseScrollAxis.y = scrollEvent.y;

        OnMouseScroll( m_v2MouseScrollAxis );
    }

    if ( event.type == SDL_MOUSEBUTTONDOWN )
    {
        auto mouseButtonEvent = event.button;

        m_umMouseButtonState.insert_or_assign(
            (MouseButton) ( mouseButtonEvent.button + 1 ), ButtonState::Pressed );

        OnMouseDown( (MouseButton) ( mouseButtonEvent.button + 1 ), m_v2MouseMoveDelta );
    }

    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        auto mouseButtonEvent = event.button;

        m_umMouseButtonState.insert_or_assign(
            (MouseButton) ( mouseButtonEvent.button + 1 ), ButtonState::Released );

        OnMouseDown( (MouseButton) ( mouseButtonEvent.button + 1 ), m_v2MouseMoveDelta );
    }
}

void InputManager::UpdateKeyboardState( float fDelta )
{
}