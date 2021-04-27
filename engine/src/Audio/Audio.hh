#pragma once

#include <cinttypes>

class Audio {
public:
    void Play();
    void Stop();
    void Pause();
    void Destroy();

    void SetVolume();

private:
    uint32_t m_hSource;
};
