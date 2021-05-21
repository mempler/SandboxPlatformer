#pragma once

#include <cstdint>

#include <Kokoro/Utility/BitFlags.hh>

enum class ButtonState : bool
{
    Released,
    Pressed
};

enum class KeyMod : uint8_t
{
    None = 0,
    SHIFT = 1 << 0,
    CONTROL = 1 << 1,
    ALT = 1 << 2,
    SUPER = 1 << 3, /* Windows Key */
    CAPS_LOCK = 1 << 4,
    NUM_LOCK = 1 << 5,
};
BitFlags( KeyMod );

enum class Key : uint16_t
{
    Key_UNKNOWN = 0,

    Key_F1 = 1,
    Key_F2 = 2,
    Key_F3 = 3,
    Key_F4 = 4,
    Key_F5 = 5,
    Key_F6 = 6,
    Key_F7 = 7,
    Key_F8 = 8,
    Key_F9 = 9,
    Key_F10 = 10,
    Key_F11 = 11,
    Key_F12 = 12,

    Key_ENTER = 13,
    Key_TAB = 14,
    Key_BACKSPACE = 15,
    Key_INSERT = 16,
    Key_DELETE = 17,
    Key_RIGHT = 18,
    Key_LEFT = 19,
    Key_DOWN = 20,
    Key_UP = 21,
    Key_PAGE_UP = 22,
    Key_PAGE_DOWN = 23,
    Key_HOME = 24,
    Key_END = 25,
    Key_CAPS_LOCK = 26,
    Key_SCROLL_LOCK = 27,
    Key_NUM_LOCK = 28,
    Key_PRINT_SCREEN = 29,
    Key_PAUSE = 30,
    Key_MENU = 31,

    Key_SPACE = 32,
    Key_APOSTROPHE = 39, /* ' */
    Key_COMMA = 44,      /* , */
    Key_MINUS = 45,      /* - */
    Key_PERIOD = 46,     /* . */
    Key_SLASH = 47,      /* / */
    Key_0 = 48,
    Key_1 = 49,
    Key_2 = 50,
    Key_3 = 51,
    Key_4 = 52,
    Key_5 = 53,
    Key_6 = 54,
    Key_7 = 55,
    Key_8 = 56,
    Key_9 = 57,
    Key_SEMICOLON = 59, /* ; */
    Key_EQUAL = 61,     /* = */
    Key_A = 65,
    Key_B = 66,
    Key_C = 67,
    Key_D = 68,
    Key_E = 69,
    Key_F = 70,
    Key_G = 71,
    Key_H = 72,
    Key_I = 73,
    Key_J = 74,
    Key_K = 75,
    Key_L = 76,
    Key_M = 77,
    Key_N = 78,
    Key_O = 79,
    Key_P = 80,
    Key_Q = 81,
    Key_R = 82,
    Key_S = 83,
    Key_T = 84,
    Key_U = 85,
    Key_V = 86,
    Key_W = 87,
    Key_X = 88,
    Key_Y = 89,
    Key_Z = 90,
    Key_LEFT_BRACKET = 91,  /* [ */
    Key_BACKSLASH = 92,     /* \ */
    Key_RIGHT_BRACKET = 93, /* ] */
    Key_GRAVE_ACCENT = 96,  /* ` */
    Key_WORLD_1 = 161,      /* non-US #1 */
    Key_WORLD_2 = 162,      /* non-US #2 */
    Key_ESCAPE = 256,

    /* Function Keys */

    Key_F13 = 302,
    Key_F14 = 303,
    Key_F15 = 304,
    Key_F16 = 305,
    Key_F17 = 306,
    Key_F18 = 307,
    Key_F19 = 308,
    Key_F20 = 309,
    Key_F21 = 310,
    Key_F22 = 311,
    Key_F23 = 312,
    Key_F24 = 313,
    Key_F25 = 314,
    Key_KP_0 = 320,
    Key_KP_1 = 321,
    Key_KP_2 = 322,
    Key_KP_3 = 323,
    Key_KP_4 = 324,
    Key_KP_5 = 325,
    Key_KP_6 = 326,
    Key_KP_7 = 327,
    Key_KP_8 = 328,
    Key_KP_9 = 329,
    Key_KP_DECIMAL = 330,
    Key_KP_DIVIDE = 331,
    Key_KP_MULTIPLY = 332,
    Key_KP_SUBTRACT = 333,
    Key_KP_ADD = 334,
    Key_KP_ENTER = 335,
    Key_KP_EQUAL = 336,
    Key_LEFT_SHIFT = 340,
    Key_LEFT_CONTROL = 341,
    Key_LEFT_ALT = 342,
    Key_LEFT_SUPER = 343,
    Key_RIGHT_SHIFT = 344,
    Key_RIGHT_CONTROL = 345,
    Key_RIGHT_ALT = 346,
    Key_RIGHT_SUPER = 347,
};

enum class MouseButton : uint8_t
{
    NONE,
    BTN_1 = 1 << 0,
    BTN_2 = 1 << 1,
    BTN_3 = 1 << 2,
    BTN_4 = 1 << 3,
    BTN_5 = 1 << 4,
    BTN_6 = 1 << 5,
    BTN_7 = 1 << 6,
    BTN_8 = 1 << 7,
    BTN_LAST = BTN_1,
    BTN_LEFT = BTN_2,
    BTN_RIGHT = BTN_3,
    BTN_MIDDLE = BTN_4,
};

BitFlags( MouseButton );
