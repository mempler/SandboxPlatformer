#include "pch.hh"

#include "Audio/AudioChannel.hh"

AudioChannel::AudioChannel(Identifier const &rIdent) : m_Identifier(rIdent) {
}

Audio *AudioChannel::CreateAudio(Identifier const &rIdent, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight) {
    return &m_vLinkedAudio.emplace_back(rIdent, hSrcLeft, hSrcRight, hBufferLeft, hBufferRight);
}
