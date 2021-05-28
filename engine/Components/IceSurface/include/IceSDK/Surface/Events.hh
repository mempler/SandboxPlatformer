#pragma once

#include <cstdint>

#include "KeyTable.hh"

namespace IceSDK
{
    enum class SurfaceEventType : uint8_t
    {
        //
        // No event needs to be proceeded
        //
        None,

        //
        // Surface
        //
        Exit,
        Resize,

        //
        // Pointer
        //
        PointerMove,
        PointerTouch,

        // Keyboard
        KeyUpdate,
        CharSend,
    };

    namespace _internal
    {
        struct EmptyEvent
        {
            SurfaceEventType Type;
        };

        struct ResizeEvent
        {
            SurfaceEventType Type;

            uint16_t Width;
            uint16_t Height;
        };

        struct PointerMoveEvent
        {
            SurfaceEventType Type;

            uint16_t X;
            uint16_t Y;

            IceSDK::KeyTable::ButtonState State;
            IceSDK::KeyTable::MouseButton Button;
        };

        struct KeyboardEvent
        {
            SurfaceEventType Type;

            IceSDK::KeyTable::Key Key;
            IceSDK::KeyTable::KeyMod Mod;
            IceSDK::KeyTable::ButtonState State;
            int Char;
        };
    }  // namespace _internal

    union SurfaceEvent
    {
        SurfaceEventType Type;

        _internal::ResizeEvent Resize;
        _internal::PointerMoveEvent Pointer;
        _internal::EmptyEvent Exit;
        _internal::KeyboardEvent Keyboard;
    };
}  // namespace IceSDK
