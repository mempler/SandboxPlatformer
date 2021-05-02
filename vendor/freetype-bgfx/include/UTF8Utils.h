//
// Created on May 2nd 2021 by loanselot1.
//

#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace ftbgfx {
    size_t utf8_surrogate_len(const char *character);
    size_t utf8_strlen(const char *string);
    uint32_t utf8_to_utf32(const char *character);
} // namespace ftbgfx
