#include "pch.hh"

#include "NetworkInspector.hh"

#include "imgui.h"

#include "Core/Engine.hh"
#include "Core/Utils/Memory.hh"

#include "Game/Game.hh"
#include "Game/Network/NetClient.hh"

#include "bgfx/bgfx.h"

#include <cstdint>

static ImVec4 GetColor_NetworkInspector( const char *szState )
{
    if ( strcmp( szState, "Disconnected" ) == 0 )
    {
        return { 1.0, 0.3, 0.3, 1.0 };
    }

    if ( strcmp( szState, "Connected" ) == 0 )
    {
        return { 0.3, 1.0, 0.3, 1.0 };
    }

    if ( strcmp( szState, "Connecting" ) == 0 )
    {
        return { 1.0, 1.0, 0.3, 1.0 };
    }

    return { 1.0, 1.0, 1.0, 1.0 };
}

void NetworkInspector::Draw()
{
    ImGui::Begin( "Network Inspector", &m_bIsShowing );

    if ( ImGui::CollapsingHeader( "State Log" ) )
    {
        auto num_offset = ImGui::CalcTextSize( "9999 " );
        auto highest_offset = ImGui::CalcTextSize( "Disconnected" );
        auto arrow_offset = ImGui::CalcTextSize( " -> " );

        for ( int i = m_vStateLog.size(); i > 0; i-- )
        {
            auto &[ before, after ] = m_vStateLog.at( i - 1 );

            ImGui::Text( "%i", i );
            ImGui::SameLine( num_offset.x );
            ImGui::TextColored( GetColor_NetworkInspector( before ), "%s", before );
            ImGui::SameLine( highest_offset.x + num_offset.x );
            ImGui::Text( " -> " );
            ImGui::SameLine( arrow_offset.x + highest_offset.x + num_offset.x );
            ImGui::TextColored( GetColor_NetworkInspector( after ), "%s", after );
        }
    }

    if ( ImGui::CollapsingHeader( "Packet Log" ) )
    {
        auto num_offset = ImGui::CalcTextSize( "9999 " );
        auto base_offset = ImGui::CalcTextSize( "client " );
        auto arrow_offset = ImGui::CalcTextSize( " ->" );

        for ( int i = m_vPacketLog.size(); i > 0; i-- )
        {
            auto &[ from, to, type, size ] = m_vPacketLog.at( i - 1 );

            auto ssize = fmt::format( " ({})", Memory::SizeInText( size ) );

            auto type_offset = ImGui::CalcTextSize( type );

            ImGui::Text( "%i", i );

            ImGui::SameLine( num_offset.x );
            ImGui::Text( "%s", from );

            ImGui::SameLine( num_offset.x + base_offset.x );
            ImGui::Text( "->" );

            ImGui::SameLine( num_offset.x + base_offset.x + arrow_offset.x );
            ImGui::Text( "%s:", to );

            ImGui::SameLine( num_offset.x + ( base_offset.x * 2 ) + arrow_offset.x );
            ImGui::TextColored( { 1.0, 1.0, 0.3, 1.0 }, " %s ", type );

            ImGui::SameLine( num_offset.x + ( base_offset.x * 2 ) + arrow_offset.x
                             + type_offset.x );
            ImGui::TextColored( { 0.3, 1.0, 0.3, 1.0 }, " %s", ssize.c_str() );
        }
    }

    ImGui::End();
}

void NetworkInspector::HookConnectionState( ConnectionState eState )
{
    m_vStateLog.push_back( { ConnectionState_ToString( m_eOldState ),
                             /*^->*/ ConnectionState_ToString( eState ) } );

    m_eOldState = eState;
}

void NetworkInspector::HookSendPacket( PacketType eType, size_t sSize )
{
    m_vPacketLog.push_back( { "Client", "Server", PacketType_ToString( eType ), sSize } );
}

void NetworkInspector::HookRecievePacket( PacketType eType, size_t sSize )
{
    m_vPacketLog.push_back( { "Server", "Client", PacketType_ToString( eType ), sSize } );
}