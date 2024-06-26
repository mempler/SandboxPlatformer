#pragma once

#include <unordered_map>

#include "Core/Utils/Identifier.hh"
#include "Core/Utils/Logger.hh"

#include <Tracy.hpp>

#include <cstddef>
#include <iterator>

template <typename TResource>
class IResourceLoader
{
  public:
    virtual void Load( TResource *pDest, Identifier const &identifier ) = 0;
};

template <typename TResource, typename ILoader>
class IResourceManager
{
  public:
    TResource *Load( Identifier const &identifier )
    {
        ZoneScoped;

        if ( m_umResources.find( identifier ) != m_umResources.end( ) )
            return &m_umResources.at( identifier );

        Console::Info( "Loading resource {}", identifier );

        TResource *resource = CreateEmpty( identifier );
        m_Loader.Load( resource, identifier );

        return &m_umResources.at( identifier );
    }

    void Destroy( TResource **ppResource )
    {
        ZoneScoped;

        if ( ppResource == nullptr ) return;

        if ( *ppResource == nullptr ) return;

        m_umResources.erase( **ppResource );

        *ppResource = nullptr;
    }

    void Destroy( Identifier const &identifier )
    {
        ZoneScoped;

        m_umResources.erase( identifier );
    }

    bool Has( Identifier const &identifier )
    {
        ZoneScoped;

        return m_umResources.find( identifier ) != m_umResources.end( );
    }

    typename std::unordered_map<Identifier, TResource>::iterator begin( )
    {
        ZoneScoped;
        return m_umResources.begin( );
    }
    typename std::unordered_map<Identifier, TResource>::iterator end( )
    {
        ZoneScoped;
        return m_umResources.end( );
    }

  protected:
    TResource *CreateEmpty( Identifier const &identifier )
    {
        ZoneScoped;
        m_umResources [ identifier ] = { };

        return &m_umResources.at( identifier );
    }

  private:
    ILoader m_Loader;
    std::unordered_map<Identifier, TResource> m_umResources;
};
