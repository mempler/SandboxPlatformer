#pragma once

#include "AudioLoader.hh"

class WavAudioLoader : public AudioLoader::BaseAudioLoader
{
  public:
    AudioDescriptor LoadBinary( std::vector<uint8_t> const &vData ) override;
};