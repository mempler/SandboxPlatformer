#include "Server.hh"

#include "Ascii.h"
#include "Server/SPlayer/SPlayer.hh"

#include "Core/Utils/Limiter.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Timer.hh"

#include "Game/Network/Packets/AvatarPacket.hh"
#include "Game/Network/Packets/AvatarStatePacket.hh"
#include "Game/Network/Packets/ItemDBPacket.hh"
#include "Game/Network/Packets/WorldPacket.hh"

#include "Network/Packet.hh"

#include "Kokoro/Memory/Buffer.hh"

#include <enet/enet.h>

#define TICK_SPEED 60

void Server::Init()
{
    Console::Init();

    Console::Info( "This server is running \r\n{}", g_ASCII );
    Console::Info( "Running: IceServer" );
    Console::Info( "Version: {}", "1.0.0" );

    // PREINIT
    m_Network.OnStateChange.connect<&Server::OnStateChange>( this );
    m_Network.OnPacket.connect<&Server::OnPacket>( this );

    ENetAddress address = { 0 };
    address.host = ENET_HOST_ANY;
    address.port = 27015;

    m_Network.InitServer( address, 32 );

    Console::Info( "Server listening on port {}", address.port );

    // TODO: actual world loading & items.dat
    Console::Info( "Loading items.dat..." );

    Item rock;
    rock.uItemX = 0;
    rock.uItemY = 0;
    rock.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( rock );

    Item dirt;
    dirt.uItemX = 1;
    dirt.uItemY = 0;
    dirt.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( dirt );

    Item grass;
    grass.uItemX = 2;
    grass.uItemY = 0;
    grass.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( grass );

    Console::Info( "Creating an empty world..." );

    m_World.Init( 100, 60 );

    for ( uint16_t x = 0; x < 100; x++ )
    {
        m_World.PlaceFore( 3, x, 0 );
        for ( uint16_t y = 1; y < 60; y++ )
        {
            m_World.PlaceFore( 2, x, y );
        }
    }

    Console::Info( "Done!" );
}

void Server::Run()
{
    Init();

    Timer timer;
    FrameLimiter<TICK_SPEED> limiter;
    while ( true )
    {
        double elapsed = timer.elapsed();

        Tick( elapsed );

        timer.reset();
        limiter.Wait();
    }
}

void Server::Tick( float fDeltaTime )
{
    // Poll messages

    m_Network.Tick();
}

void Server::OnStateChange( NetClientPtr pClient, ConnectionState eState )
{
    switch ( eState )
    {
    case ConnectionState::Connected:
    {
        Console::Info( "A peer has established a connection!" );

        SPlayer *player = new SPlayer( pClient );
        pClient->m_ExtraData = player;
        m_vPlayers.push_back( player );

        break;
    }
    case ConnectionState::Connecting:
    {
        Console::Info( "A new peer is connecting!" );

        // A client is attempting to connect
        // Try to accept the connection.
        // if ( m_Listener.AcceptConnection( pClient ) )
        // {
        //     pClient->Disconnect( "Unexpected closure!!" );
        //     Console::Error( "Couldn't accept connection. (Why?)" );
        //     break;
        // }

        break;
    }
    case ConnectionState::Disconnected:
    {
        Console::Info( "A peer disconnected..." );
        // data gets destroyed i think
        break;
    }
    }
}

void Server::OnPacket( NetClientPtr pClient, PacketHeader header,
                       Kokoro::Memory::Buffer buffer )
{
    auto pPlayer = (SPlayer *) pClient->m_ExtraData;

    switch ( header.m_eType )
    {
    case PacketType::CLN_RequestItemDB:
    {
        Console::Trace( "A peer requested the ItemDB to be send" );

        // TODO: load itemdb from a file
        Packets::SND_ItemDB packet;
        packet.m_Object = &m_ItemInfoManager;

        pClient->Send( packet );
        break;
    }

    case PacketType::CLN_RequestWorld:
    {
        Console::Trace( "A peer requested the world to be send" );

        // TODO: load worlds from a file
        Packets::SND_World packet;
        packet.m_Object = &m_World;

        pClient->Send( packet );

        break;
    }

    case PacketType::CLN_RequestAvatar:
    {
        Console::Trace( "A peer requested the local avatar to be send" );

        // yeah yeah TODO
        for ( auto &avatar : m_World.m_vAvatars )
        {
            Packets::AvatarReceiveData data;
            data.SetAvatar( avatar, false );

            Packets::SND_Avatar packet {};
            packet.m_Object = &data;

            pClient->Send( packet );

            Console::Log( "Sent local ID: {}", data.m_ID );
        }

        Avatar *avatar = new Avatar;
        avatar->m_v3Position = { 200.f, 100.f, 7.f };
        avatar->m_ID = pPlayer->m_PID;

        m_World.AvatarOnEnter( avatar );

        Packets::AvatarReceiveData data;
        data.SetAvatar( avatar, true );

        Packets::SND_Avatar packet {};
        packet.m_Object = &data;

        pClient->Send( packet );

        Console::Log( "Sent local ID: {}", data.m_ID );
        break;
    }

    case PacketType::CLN_RequestAvatarState:
    {
        Packets::AvatarStateData receivedData;
        receivedData.Unpack( buffer );

        for ( auto avatar : m_World.m_vAvatars )
        {
            if ( avatar->m_ID == pPlayer->m_PID )
            {
                receivedData.InitAvatar( avatar );
            }
        }

        for ( auto avatar : m_World.m_vAvatars )
        {
            if ( avatar->m_ID != pPlayer->m_PID ) continue;

            Packets::AvatarStateData data;
            data.SetAvatar( avatar );

            Packets::SND_AvatarState packet;
            packet.m_Object = &data;

            for ( auto &client : m_Network.m_vClients )
            {
                if ( client == pClient ) continue;
                client->Send( packet );
            }
        }
        break;
    }

    case PacketType::CLN_RequestLogin:
    {

        break;
    }

    default:
    {
        Console::Warn( "Unimplemented Packet: {}!", (int) header.m_eType );
        break;
    }
    }
}
