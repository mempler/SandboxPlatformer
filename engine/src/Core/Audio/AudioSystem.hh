#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "Core/Audio/AudioChannel.hh"
#include "Core/Utils/Identifier.hh"

class AudioSystem {
public:
    ~AudioSystem();

    AudioChannel *CreateChannel(Identifier const &rIdent);

    Audio *LoadStereoAudio(AudioChannel *pChannel, Identifier const &rIdent, Identifier const &rIdentLeft, Identifier const &rIdentRight);
    Audio *LoadMonoAudio(AudioChannel *pChannel, Identifier const &rIdent);

    void Init();

private:
    std::vector<AudioChannel> m_vAudioChannels;

    ALCdevice *m_alDevice;   // Active Audio Device
    ALCcontext *m_alContext; // Active OpenAL Context (once per thread)
};
