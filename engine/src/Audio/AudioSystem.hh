#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "AudioChannel.hh"

#include "Core/Utils/Identifier.hh"

class AudioSystem {
public:
    ~AudioSystem();

    AudioChannel *CreateChannel(Identifier const &rIdent);

    Audio *LoadAudio(AudioChannel *pChannel, Identifier const &rIdent);

    void Init();

private:
    ALCdevice *m_alDevice;   // Active Audio Device
    ALCcontext *m_alContext; // Active OpenAL Context (once per thread)
};
