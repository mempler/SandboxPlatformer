#pragma once

#include <string_view>

#include "Kokoro/Memory/Span.hh"

#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <string>

// Please note that this file does NOT own the string!
class Identifier
{
  public:
    // E.G file://Texture.png
    // E.G pkg://Texture.png
    // E.G http://Texture.png
    constexpr Identifier( std::string_view const &svUri )
    {
        assert( svUri.size() < 64 && "An identifier must be under 64 bytes!" );

        // No constexpr std::copy :c
        for ( size_t i = 0; i < svUri.size(); i++ )
        {
            m_vString [ i ] = svUri [ i ];
        }

        auto uri = std::string_view( m_vString.data() );

        constexpr std::string_view protoEnd( "://" );

        auto proto_end = uri.find_first_of( protoEnd );
        if ( proto_end == std::string_view::npos )
        {
            m_sProtocol = uri;
            return;
        }

#if _MSC_FULL_VER  // m$ft moment
        m_sProtocol = std::string_view( &*uri.begin(), proto_end );
        m_sPath = std::string_view( &*uri.begin() + proto_end + protoEnd.size() );
#else
        m_sProtocol = std::string_view( uri.begin(), proto_end );
        m_sPath = std::string_view( uri.begin() + proto_end + protoEnd.size() );
#endif
    }

    constexpr Identifier( char *szUri ) : Identifier( std::string_view( szUri ) )
    {
    }
    constexpr Identifier( const char *szUri ) : Identifier( std::string_view( szUri ) )
    {
    }
    Identifier( std::string const &szUri ) : Identifier( std::string_view( szUri ) )
    {
    }

    constexpr std::string_view const &Protocol() const
    {
        return m_sProtocol;
    }

    constexpr std::string_view const &Path() const
    {
        return m_sPath;
    }

    constexpr std::string_view Raw() const
    {
        return std::string_view( m_vString.data() );
    }

    constexpr operator const char *() const
    {
        return m_vString.data();
    }

    constexpr operator std::string_view() const
    {
        return Raw();
    }

    // Dumb piece of shit
    operator Kokoro::Memory::Span<uint8_t>() const
    {
        return Kokoro::Memory::Span<uint8_t> { (uint8_t *) m_vString.data(),
                                               m_vString.size() };
    }

    constexpr bool operator==( const Identifier &other ) const
    {
        return m_vString == other.m_vString;
    }

  private:
    std::array<char, 64> m_vString {};

    std::string_view m_sProtocol = "", m_sPath = "";
};

static constexpr Identifier EmptyIdentifier = Identifier( "engine://empty" );

// std::unordered_map support
template <>
struct std::hash<Identifier>
{
    std::size_t operator()( const Identifier &s ) const noexcept
    {
        std::size_t h1 = std::hash<std::string_view> {}( s.Raw() );

        return h1;
    }
};
