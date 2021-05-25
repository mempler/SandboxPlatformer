#include "SPlayer.hh"

SPlayer::SPlayer( NetClientPtr pClient ) : m_pClient( pClient )
{
    m_PID = s_uNetID++;
}

NetClientPtr SPlayer::ClientHandle()
{
    return m_pClient;
}

bool SPlayer::PackLogin( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    buffer.Push<uint8_t>( m_sName.length() );
    buffer.Push( m_sName );
    buffer.Push( m_UID );

    return true;
}

bool SPlayer::UnpackLogin( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;
    if ( !buffer.can_read( 20 ) ) return false;

    uint8_t nameLen = buffer.Pop<uint8_t>( 1 );
    m_sName = buffer.Pop<std::string>( nameLen );
    m_UID = buffer.Pop<uint32_t>( 12 );

    return true;
}