#pragma once

#include "Core/Utils/Identifier.hh"

#include <glm/glm.hpp>

#include <cstdint>

class AudioChannel;
class Audio
{
  public:
    Audio( AudioChannel *pChannel, Identifier const &rIdent, uint32_t hSrcLeft,
           uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight );

    void Play();
    void Stop();
    void Pause();
    void Destroy();

    float GetPitch();
    void SetPitch( float fPitch );

    float GetVolume();
    void SetVolume( float fVolume );

    void SetPosition( glm::vec3 v3Position );

  private:
    AudioChannel *m_pChannel;
    Identifier m_Identifier;

    float m_fVolume = 1.0f;
    float m_fPitch = 1.0f;

    uint32_t m_hBufferLeft;
    uint32_t m_hBufferRight;

    uint32_t m_hSourceLeft;
    uint32_t m_hSourceRight;

    void __SetVolume( float fVolume );
};
