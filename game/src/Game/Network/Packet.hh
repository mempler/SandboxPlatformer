#pragma once

#include <Kokoro/Memory/Buffer.hh>
#include <steam/isteamnetworkingsockets.h>

#include <cstdint>

enum class PacketType : uint16_t
{
    CLN_RequestItemDB,
    SRV_SendItemDB,

    CLN_RequestWorld,
    SRV_SendWorld,
};

enum class PacketFlags
{
    None = 0
};

struct PacketHeader
{
    PacketType m_uType;
    PacketFlags m_eFlags;
    uint8_t m_Padding [ 20 ];  // In case we want to add smth in the header
    uint32_t m_uSize;
};

struct IBasePacket
{
    virtual void Pack( Kokoro::Memory::Buffer &buffer ) = 0;
    virtual void Unpack( Kokoro::Memory::Buffer &buffer ) = 0;

    void SendTo( ISteamNetworkingSockets *pSteamSockets, HSteamNetConnection &hConn )
    {
        Kokoro::Memory::Buffer buffer;

        auto data = buffer.data();
        auto size = buffer.end() - buffer.begin();

        // Send buffer to client
        pSteamSockets->SendMessageToConnection(
            hConn, data, size, k_nSteamNetworkingSend_Reliable, nullptr );
    }
};
