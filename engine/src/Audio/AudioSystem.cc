#include "pch.hh"

#include "Audio/AudioSystem.hh"

#include "Core/Utils/Logger.hh"

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
    return nullptr;
}

Audio *LoadAudio(AudioChannel *pChannel, Identifier const &rIdent) {
    return nullptr;
}