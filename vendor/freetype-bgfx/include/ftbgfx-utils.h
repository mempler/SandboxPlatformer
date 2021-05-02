//
// Created on May 2nd 2021 by loanselot1.
//

#pragma once

namespace ftbgfx {
    typedef void (*error_callback_t)(const char *fmt, ...);
    extern error_callback_t log_error;

    /**
     * Prints input to stderr
     * This is fallback function for error reporting if ftgl_set_error_callback() wans't called
     *
     * @param fmt       cstring to be printed matching C-style printing syntax
     * @param ...       va_list fmt supplying arguments
     */
    void error_callback_default(const char *fmt, ...);

    /**
     * Set function to call on error handling
     * This is fallback function for error reporting if ftgl_set_error_callback() wans't called
     *
     * @param error_cb  callback function to call on error, see error_callback_default for reference
     */
    void set_error_callback(error_callback_t error_cb);
} // namespace ftbgfx
