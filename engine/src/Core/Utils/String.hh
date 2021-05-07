#pragma once

namespace String {
    inline std::string Format(const char *szStr, ...) {
        char fmt[1024] = { 0 };
        va_list va;
        va_start(va, szStr);
        vsnprintf(fmt, 1024, szStr, va);
        va_end(va);
        return std::string(fmt);
    }
} // namespace String
