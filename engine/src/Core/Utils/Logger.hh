#pragma once

// Some basic logger implementation that can be removed 
// later on, I am just too lazy to add some logger lib -loanselot

#include "pch.hh"

#define LOG(x, ...) logger::Logf(x "\n", ##__VA_ARGS__)

// clang-format off
#ifdef _DEBUG
#define LOG_INFO(x, ...)        Logger::Logf(" ---- | INFO  | " x "\n", ##__VA_ARGS__)
#define LOG_WARN(x, ...)        Logger::Logf(" ---- | WARN  | " x "\n", ##__VA_ARGS__)
#define LOG_ERROR(x, ...)       Logger::Logf(" ---- | ERROR | " x "\n", ##__VA_ARGS__)
#define DEBUG_LOG_INFO(x, ...)  Logger::Logf("DEBUG | INFO  | [%s:%d - '%s']: " x "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define DEBUG_LOG_WARN(x, ...)  Logger::Logf("DEBUG | WARN  | [%s:%d - '%s']: " x "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define DEBUG_LOG_ERROR(x, ...) Logger::Logf("DEBUG | ERROR | [%s:%d - '%s']: " x "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define LOG_INFO(x, ...)        Logger::Logf(Xor("INFO  | " x "\n"), ##__VA_ARGS__)
#define LOG_WARN(x, ...)        Logger::Logf(Xor("WARN  | " x "\n"), ##__VA_ARGS__)
#define LOG_ERROR(x, ...)       Logger::Logf(Xor("ERROR | " x "\n"), ##__VA_ARGS__)
#define DEBUG_LOG_INFO(x, ...)  (void *)0;
#define DEBUG_LOG_WARN(x, ...)  (void *)0;
#define DEBUG_LOG_ERROR(x, ...) (void *)0;
#endif
// clang-format on

namespace Logger {
    void Clear();
    void Logf(const char *output, ...);
} // namespace logger