#pragma once

#include "AudioLoader.hh"

class WavAudioLoader : public AudioLoader {
public:
    AudioDescriptor LoadBinary(std::vector<uint8_t> const &vData) override;
};