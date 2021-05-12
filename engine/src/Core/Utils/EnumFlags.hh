#pragma once

#define EnumFlags(NAME)                                                      \
    inline NAME operator|(NAME a, NAME b) {                                  \
        return static_cast<NAME>(static_cast<int>(a) | static_cast<int>(b)); \
    }                                                                        \
    inline NAME operator|(NAME a, int b) {                                   \
        return static_cast<NAME>(static_cast<int>(a) | static_cast<int>(b)); \
    }                                                                        \
    inline NAME operator|(int a, NAME b) {                                   \
        return static_cast<NAME>(static_cast<int>(a) | static_cast<int>(b)); \
    }                                                                        \
    inline int operator&(NAME a, NAME b) {                                   \
        return static_cast<int>(static_cast<int>(a) & static_cast<int>(b));  \
    }                                                                        \
    inline int operator&(NAME a, int b) {                                    \
        return static_cast<int>(static_cast<int>(a) & static_cast<int>(b));  \
    }                                                                        \
    inline int operator&(int a, NAME b) {                                    \
        return static_cast<int>(static_cast<int>(a) & static_cast<int>(b));  \
    }                                                                        \
    inline NAME &operator|=(NAME &a, NAME b) {                               \
        return a = a | b;                                                    \
    }                                                                        \
    inline NAME &operator|=(NAME &a, int b) {                                \
        return a = a | b;                                                    \
    }