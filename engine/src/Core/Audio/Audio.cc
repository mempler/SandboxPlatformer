#include "pch.hh"

#include "Core/Audio/Audio.hh"

#include "Core/Audio/AudioChannel.hh"
#include "Core/Audio/Helper.hh"

Audio::Audio(AudioChannel *pChannel, Identifier const &rIdent, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight)
                                                                                                    : m_pChannel(pChannel), m_Identifier(rIdent), m_hSourceLeft(hSrcLeft),
                                                                                                      m_hBufferLeft(hBufferLeft), m_hSourceRight(hSrcRight),
                                                                                                      m_hBufferRight(hBufferRight) {
}

void Audio::Play() {
    alSourcePlay(m_hSourceLeft);
    alSourcePlay(m_hSourceRight);

    alSourcei(m_hSourceLeft, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(m_hSourceRight, AL_SOURCE_RELATIVE, AL_TRUE);

    alSource3f(m_hSourceLeft, AL_POSITION, -1.0f, 0.0f, 0.0f);
    alSource3f(m_hSourceRight, AL_POSITION, 1.0f, 0.0f, 0.0f);

    // Reapply Pitch / Volume
    SetPitch(m_fPitch);
    SetVolume(m_fVolume);

    AL_ERROR_CHECK();
}

void Audio::Stop() {
    alSourceStop(m_hSourceLeft);
    alSourceStop(m_hSourceRight);
}

void Audio::Pause() {
    alSourcePause(m_hSourceLeft);
    alSourcePause(m_hSourceRight);
}

float Audio::GetPitch() {
    return m_fPitch;
}
void Audio::SetPitch(float fPitch) {
    m_fPitch = fPitch;

    alSourcef(m_hSourceLeft, AL_PITCH, m_fPitch);
    alSourcef(m_hSourceRight, AL_PITCH, m_fPitch);
}

float Audio::GetVolume() {
    return m_fVolume;
}
void Audio::SetVolume(float fVolume) {
    m_fVolume = fVolume;

    alSourcef(m_hSourceLeft, AL_GAIN, m_fVolume * m_pChannel->GetVolume());
    alSourcef(m_hSourceRight, AL_GAIN, m_fVolume * m_pChannel->GetVolume());
}

void Audio::SetPosition(glm::vec3 v3Position) {
    { // X
        float volumeL = 1 - ((v3Position.x + 1) / 2);
        float volumeR = ((v3Position.x + 1) / 2);

        //printf("L: %f R: %f\n", volumeL, volumeR);

        alSourcef(m_hSourceLeft, AL_GAIN, volumeL * m_fVolume);
        alSourcef(m_hSourceRight, AL_GAIN, volumeR * m_fVolume);
    }

    { // YZ
        alSource3f(m_hSourceLeft, AL_POSITION, -1.0f, v3Position.y, v3Position.z);
        alSource3f(m_hSourceRight, AL_POSITION, 1.0f, v3Position.y, v3Position.z);
    }
}
