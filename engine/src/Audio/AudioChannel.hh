#pragma once

// an Audio channel is the definition of a Master sound file
// basically you can adjust the volume, position etc etc for all sound files
// that are linked to this Channel

#include "Audio.hh"

#include <vector>

class AudioChannel {
public:
    void SetVolume(float fVolume);

private:
    std::vector<Audio> m_vLinkedAudio;
};
