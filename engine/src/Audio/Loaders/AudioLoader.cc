#include "pch.hh"

#include "AudioLoader.hh"

#include "WavAudioLoader.hh"

#include <memory>

AudioDescriptor LoadAudio(AudioLoaderType eType, std::vector<uint8_t> const &vData) {
    switch (eType) {
        case AudioLoaderType::WAV: {
            WavAudioLoader loader;

            return loader.LoadBinary(vData);
        } break;

        default: {
            CHECK(false, "Unimplemented Audio File Format!");
        } break;
    }

    return {};
}
