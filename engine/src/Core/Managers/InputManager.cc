#include "InputManager.hh"

void InputManager::PumpSDL2Event(SDL_Event &pEvent) {
    if (pEvent.type == SDL_KEYDOWN || pEvent.type == SDL_KEYUP) {
        auto keyEvent = pEvent.key.keysym;

        uint8_t keyMod = KeyMod::None;

        if ((keyEvent.mod & KMOD_LSHIFT) > 0) {
            keyMod |= KeyMod::SHIFT;
        }

        if ((keyEvent.mod & KMOD_RSHIFT) > 0) {
            keyMod |= KeyMod::SHIFT;
        }

        if ((keyEvent.mod & KMOD_LCTRL) > 0) {
            keyMod |= KeyMod::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RCTRL) > 0) {
            keyMod |= KeyMod::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RALT) > 0) {
            keyMod |= KeyMod::ALT;
        }

        if ((keyEvent.mod & KMOD_LALT) > 0) {
            keyMod |= KeyMod::ALT;
        }

        if ((keyEvent.mod & KMOD_LGUI) > 0) {
            keyMod |= KeyMod::SUPER;
        }

        if ((keyEvent.mod & KMOD_RGUI) > 0) {
            keyMod |= KeyMod::SUPER;
        }

        if ((keyEvent.mod & KMOD_NUM) > 0) {
            keyMod |= KeyMod::NUM_LOCK;
        }

        if ((keyEvent.mod & KMOD_CAPS) > 0) {
            keyMod |= KeyMod::CAPS_LOCK;
        }

        m_iKeyMods = (KeyMod)keyMod;
    }

    if (pEvent.type == SDL_KEYDOWN) {
        auto keyEvent = pEvent.key.keysym;

        m_umKeyState.insert_or_assign((Key)SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode), ButtonState::Pressed);
    }

    if (pEvent.type == SDL_KEYUP) {
        auto keyEvent = pEvent.key.keysym;

        m_umKeyState.insert_or_assign((Key)SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode), ButtonState::Released);
    }

    if (pEvent.type == SDL_MOUSEMOTION) {
        auto motionEvent = pEvent.motion;

        m_v4MouseMoveDelta.x = motionEvent.x;
        m_v4MouseMoveDelta.y = motionEvent.y;
    }

    if (pEvent.type == SDL_MOUSEWHEEL) {
        auto scrollEvent = pEvent.wheel;

        m_v4MouseScrollAxis.x = scrollEvent.x;
        m_v4MouseScrollAxis.y = scrollEvent.y;
    }

    if (pEvent.type == SDL_MOUSEBUTTONDOWN) {
        auto mouseButtonEvent = pEvent.button;

        m_umMouseButtonState.insert_or_assign((MouseButton)(mouseButtonEvent.button + 1), ButtonState::Pressed);
    }

    if (pEvent.type == SDL_MOUSEBUTTONUP) {
        auto mouseButtonEvent = pEvent.button;

        m_umMouseButtonState.insert_or_assign((MouseButton)(mouseButtonEvent.button + 1), ButtonState::Released);
    }
}