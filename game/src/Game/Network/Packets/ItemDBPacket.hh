#pragma once

#include "Game/Managers/ItemInfoManager.hh"

#include "Network/Packet.hh"

namespace Packets
{
    struct ItemDBRequestData
    {
        bool Pack( Kokoro::Memory::Buffer &buffer )
        {
            return true;
        }

        bool Unpack( Kokoro::Memory::Buffer &buffer )
        {
            return true;
        }
    };

    typedef IBasePacket<PacketType::CLN_RequestItemDB, ItemDBRequestData> REQ_ItemDB;
    typedef IBasePacket<PacketType::SRV_SendItemDB, ItemInfoManager> SND_ItemDB;
}  // namespace Packets
