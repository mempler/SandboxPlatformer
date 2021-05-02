#include "pch.hh"

#include "Core/Audio/AudioSystem.hh"

#include "Core/Audio/Audio.hh"
#include "Core/Audio/AudioChannel.hh"
#include "Core/Audio/Helper.hh"
#include "Core/Audio/Loaders/AudioLoader.hh"
#include "Core/Utils/FileSystem.hh"
#include "Core/Utils/Logger.hh"

#include "Kokoro/FileSystem.hh"

#include <AL/al.h>

AudioSystem::~AudioSystem() {
    if (m_alContext == nullptr)
        return;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_alContext);
    alcCloseDevice(m_alDevice);
}

void AudioSystem::Init() {
    m_alDevice = alcOpenDevice(nullptr); // Select the default device

    CHECK(m_alDevice != nullptr, "Failed to open Audio Device!");

    m_alContext = alcCreateContext(m_alDevice, nullptr);
    if (m_alContext == nullptr || alcMakeContextCurrent(m_alContext) == ALC_FALSE) {
        if (m_alContext != nullptr)
            alcDestroyContext(m_alContext);

        alcCloseDevice(m_alDevice);

        CHECK(false, "Failed set a Audio Context!");
    }

    const char *name = nullptr;

    if (alcIsExtensionPresent(m_alDevice, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(m_alDevice, ALC_ALL_DEVICES_SPECIFIER);

    if (!name || alcGetError(m_alDevice) != AL_NO_ERROR)
        name = alcGetString(m_alDevice, ALC_DEVICE_SPECIFIER);

    LOG_INFO("Audio Device: %s", name);
    LOG_INFO("Successfully Initialized Audio System!");
}

AudioChannel *AudioSystem::CreateChannel(Identifier const &rIdent) {
    return &m_vAudioChannels.emplace_back(rIdent);
}

Audio *AudioSystem::LoadStereoAudio(AudioChannel *pChannel, Identifier const &rIdent, Identifier const &rIdentLeft, Identifier const &rIdentRight) {
    // TODO: stream the file instead of loading the whole thing into memory

    AudioLoader::AudioLoaderType loaderTypeLeft;
    AudioLoader::AudioLoaderType loaderTypeRight;

    if (rIdentLeft.Path().ends_with(".wav")) {
        loaderTypeLeft = AudioLoader::AudioLoaderType::WAV;
    }

    if (rIdentRight.Path().ends_with(".wav")) {
        loaderTypeRight = AudioLoader::AudioLoaderType::WAV;
    }

    uint32_t audioSrcLeft;
    uint32_t audioSrcRight{}; // fix warns

    uint32_t audioBuffLeft;
    uint32_t audioBuffRight{}; // fix warns

    AudioDescriptor descLeft;
    AudioDescriptor descRight;

    if (rIdentLeft.Protocol() == "file") {
        auto data = FileSystem::ReadBinaryFile(rIdentLeft.Path().data());
        auto eastl_data = eastl::span(data.data(), data.size());

        descLeft = AudioLoader::LoadAudio(loaderTypeLeft, eastl_data);
    }

    if (rIdentRight.Protocol() == "file") {
        auto data = FileSystem::ReadBinaryFile(rIdentRight.Path().data());
        auto eastl_data = eastl::span(data.data(), data.size());

        descRight = AudioLoader::LoadAudio(loaderTypeRight, eastl_data);
    }

    {
        alGenBuffers(1, &audioBuffLeft);
        alBufferData(audioBuffLeft, descLeft.m_vPCMFormat, descLeft.m_vPCMFrames.data(), descLeft.m_vPCMFrames.size(), descLeft.m_vPCMFrequency);

        AL_ERROR_CHECK();
        if (AL_IS_ERROR()) {
            if (audioBuffRight && alIsBuffer(audioBuffRight))
                alDeleteBuffers(1, &audioBuffRight);

            if (audioBuffLeft && alIsBuffer(audioBuffLeft))
                alDeleteBuffers(1, &audioBuffLeft);

            return nullptr;
        }
    }

    {
        alGenBuffers(1, &audioBuffRight);
        alBufferData(audioBuffRight, descRight.m_vPCMFormat, descRight.m_vPCMFrames.data(), descRight.m_vPCMFrames.size(), descRight.m_vPCMFrequency);

        AL_ERROR_CHECK();
        if (AL_IS_ERROR()) {
            if (audioBuffRight && alIsBuffer(audioBuffRight))
                alDeleteBuffers(1, &audioBuffRight);

            if (audioBuffLeft && alIsBuffer(audioBuffLeft))
                alDeleteBuffers(1, &audioBuffLeft);

            return nullptr;
        }
    }

    {
        alGenSources(1, &audioSrcLeft);
        alSourcei(audioSrcLeft, AL_BUFFER, audioBuffLeft);

        AL_ERROR_CHECK();
        if (AL_IS_ERROR()) {
            if (audioBuffRight && alIsBuffer(audioBuffRight))
                alDeleteBuffers(1, &audioBuffRight);

            if (audioBuffLeft && alIsBuffer(audioBuffLeft))
                alDeleteBuffers(1, &audioBuffLeft);

            if (audioSrcLeft && alIsSource(audioSrcLeft))
                alDeleteSources(1, &audioSrcLeft);

            if (audioSrcRight && alIsSource(audioSrcRight))
                alDeleteSources(1, &audioSrcRight);

            return nullptr;
        }
    }

    {
        alGenSources(1, &audioSrcRight);
        alSourcei(audioSrcRight, AL_BUFFER, audioBuffRight);

        AL_ERROR_CHECK();
        if (AL_IS_ERROR()) {
            if (audioBuffRight && alIsBuffer(audioBuffRight))
                alDeleteBuffers(1, &audioBuffRight);

            if (audioBuffLeft && alIsBuffer(audioBuffLeft))
                alDeleteBuffers(1, &audioBuffLeft);

            if (audioSrcLeft && alIsSource(audioSrcLeft))
                alDeleteSources(1, &audioSrcLeft);

            if (audioSrcRight && alIsSource(audioSrcRight))
                alDeleteSources(1, &audioSrcRight);

            return nullptr;
        }
    }

    return pChannel->CreateAudio(rIdent, audioSrcLeft, audioSrcRight, audioBuffLeft, audioBuffRight);
}

Audio *AudioSystem::LoadMonoAudio(AudioChannel *pChannel, Identifier const &rIdent) {
    return LoadStereoAudio(pChannel, rIdent, rIdent, rIdent); // *Emulate* Stereo
}
