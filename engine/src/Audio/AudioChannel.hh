#pragma once

// an Audio channel is the definition of a Master sound file
// basically you can adjust the volume, position etc etc for all sound files
// that are linked to this Channel

#include "Audio.hh"

#include "Core/Utils/Identifier.hh"

#include "glm/fwd.hpp"

#include <vector>

class AudioChannel {
public:
    AudioChannel(Identifier const &rIdent);

    Audio *CreateAudio(Identifier const &rIdent, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight);

private:
    Identifier m_Identifier;

    std::vector<Audio> m_vLinkedAudio;
};