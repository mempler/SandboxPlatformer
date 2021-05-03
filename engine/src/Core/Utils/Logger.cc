#include "pch.hh"

#include "Logger.hh"

#include <cstdarg>
#include <cstdio>

void Logger::Clear() {
    FILE *f = fopen("log.txt", "w");
    fclose(f);
}

void Logger::Logf(const char *output, ...) {
    FILE *f = fopen("log.txt", "a");

    va_list args;
    va_start(args, output);
    vfprintf(f, output, args);
    va_end(args);

    fclose(f);
}