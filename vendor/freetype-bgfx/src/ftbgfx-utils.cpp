#include "ftbgfx-utils.h"

#include <stdarg.h>
#include <stdio.h>

ftbgfx::error_callback_t log_error = ftbgfx::error_callback_default;

// ------------------------------------------------- error_callback_default ---
void ftbgfx::error_callback_default(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

// ----------------------------------------------------- set_error_callback ---
void ftbgfx::set_error_callback(ftbgfx::error_callback_t error_cb) {
    log_error = error_cb;
}