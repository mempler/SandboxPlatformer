#pragma once

#define AL_ERROR_CHECK()                                         \
    {                                                            \
        auto alError = alGetError();                             \
        if (alError != AL_NO_ERROR) {                            \
            LOG_ERROR("OpenAL Error: %s", alGetString(alError)); \
        }                                                        \
    }

#define AL_IS_ERROR() alGetError() != AL_NO_ERROR
