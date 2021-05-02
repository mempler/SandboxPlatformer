#include "pch.hh"

#include "WavAudioLoader.hh"

#include "AL/al.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

AudioDescriptor WavAudioLoader::LoadBinary(eastl::span<uint8_t> const &vData) {
    AudioDescriptor descriptor{};

    drwav wav;
    CHECK(drwav_init_memory(&wav, vData.data(), vData.size(), NULL), "Failed to load WAV File!");

    eastl::vector<uint8_t> frames;
    frames.resize(wav.totalPCMFrameCount * wav.channels * wav.bitsPerSample);

    size_t numberOfSamplesActuallyDecoded = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, (void *)frames.data());

    descriptor.m_vPCMFrames = eastl::move(frames);
    descriptor.m_vPCMFrequency = wav.sampleRate;

    switch (wav.channels) {
        case 1: descriptor.m_vPCMFormat = AL_FORMAT_MONO16; break;
        case 2: descriptor.m_vPCMFormat = AL_FORMAT_STEREO16; break;
        default: CHECK(false, "Unsupported Audio Channel count %i!", wav.channels); break;
    }

    drwav_uninit(&wav);

    return descriptor;
}