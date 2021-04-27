#include "pch.hh"

#include "Audio/AudioChannel.hh"

AudioChannel::AudioChannel(Identifier const &rIdent) : m_Identifier(rIdent) {
}

Audio *AudioChannel::CreateAudio(Identifier const &rIdent, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight) {
    return &m_vLinkedAudio.emplace_back(this, rIdent, hSrcLeft, hSrcRight, hBufferLeft, hBufferRight);
}

float AudioChannel::GetVolume() {
    return m_fVolume;
}
void AudioChannel::SetVolume(float fVolume) {
    m_fVolume = fVolume;

    for (auto &audio : m_vLinkedAudio) {
        audio.SetVolume(audio.GetVolume()); // Each audio file should get the volume this channel by default.
    }
}