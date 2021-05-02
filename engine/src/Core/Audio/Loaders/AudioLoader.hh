#pragma once

#include <EASTL/span.h>
#include <EASTL/vector.h>

#include <cstdint>

struct AudioDescriptor {
    uint32_t m_vPCMFrequency;
    uint32_t m_vPCMFormat;

    eastl::vector<uint8_t> m_vPCMFrames;
};

namespace AudioLoader {
    class BaseAudioLoader {
    public:
        // Should return the audio wave data
        virtual AudioDescriptor LoadBinary(eastl::span<uint8_t> const &vData) = 0;
    };

    enum class AudioLoaderType {
        WAV,
    };

    AudioDescriptor LoadAudio(AudioLoaderType eType, eastl::span<uint8_t> const &vData);
} // namespace AudioLoader
