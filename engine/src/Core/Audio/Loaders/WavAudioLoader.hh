#pragma once

#include "AudioLoader.hh"

class WavAudioLoader : public AudioLoader::BaseAudioLoader {
public:
    AudioDescriptor LoadBinary(eastl::span<uint8_t> const &vData) override;
};