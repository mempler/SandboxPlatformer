#include "pch.hh"

#include "WavAudioLoader.hh"

#include "AL/al.h"

#include "dr_wav.h"

AudioDescriptor WavAudioLoader::LoadBinary(std::vector<uint8_t> const &vData) {
    AudioDescriptor descriptor{};

    drwav wav;
    CHECK(drwav_init_memory(&wav, vData.data(), vData.size(), NULL), "Failed to load WAV File!");

    std::vector<uint8_t> frames;
    frames.resize(wav.totalPCMFrameCount * wav.channels * sizeof(uint32_t));

    size_t numberOfSamplesActuallyDecoded = drwav_read_pcm_frames_s32(&wav, wav.totalPCMFrameCount, (int32_t *)frames.data());

    drwav_uninit(&wav);

    descriptor.m_vPCMFrames = std::move(frames);
    descriptor.m_vPCMFrequency = wav.sampleRate;

    if (wav.channels == 1)
        descriptor.m_vPCMFormat = AL_FORMAT_MONO16;

    else if (wav.channels == 2)
        descriptor.m_vPCMFormat = AL_FORMAT_STEREO16;

    else
        CHECK(false, "Unsupported Audio Channel count %i!", wav.channels);

    return descriptor;
}