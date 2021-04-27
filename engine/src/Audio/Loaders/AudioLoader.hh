#pragma once

#include <cstdint>
#include <vector>

struct AudioDescriptor {
    uint32_t m_vPCMFrequency;
    uint32_t m_vPCMFormat;

    std::vector<uint8_t> m_vPCMFrames;
};

class AudioLoader {
public:
    // Should return the audio wave data
    virtual AudioDescriptor LoadBinary(std::vector<uint8_t> const &vData) = 0;
};

enum class AudioLoaderType {
    WAV,
};

AudioDescriptor LoadAudio(AudioLoaderType eType, std::vector<uint8_t> const &vData);
