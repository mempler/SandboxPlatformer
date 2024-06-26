#pragma once

#include <tuple>
#include <vector>

#include "Core/Debug/IDebugUtil.hh"

#include "Network/NetClient.hh"
#include "Network/Network.hh"
#include "Network/Packet.hh"

#include "bgfx/bgfx.h"

class NetworkInspector : public IDebugUtil
{
  public:
    NetworkInspector( Engine *pEngine ) : IDebugUtil( pEngine )
    {
    }

    void Draw() override;

    const char *Name() override
    {
        return "Network Inspector";
    }

    void HookConnectionState( ConnectionState eState );
    void HookSendPacket( PacketType eType, size_t sSize );
    void HookRecievePacket( PacketType eType, size_t sSize );

  private:
    ConnectionState m_eOldState = ConnectionState::Disconnected;

    std::vector<std::tuple<const char *, const char *, const char *, size_t>>
        m_vPacketLog;

    std::vector<std::pair<const char *, const char *>> m_vStateLog;
};
