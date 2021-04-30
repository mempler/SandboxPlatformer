#pragma once

#include <unordered_map>

#include "Core/Utils/Identifier.hh"
#include "Core/Utils/Logger.hh"

#include <cstddef>
#include <iterator>

template <typename TResource>
class IResourceLoader {
public:
    virtual void Load(TResource *pDest, Identifier const &identifier) = 0;
};

template <typename TResource, typename ILoader>
class IResourceManager {
public:
    TResource *Load(Identifier const &identifier) {
        if (m_umResources.find(identifier) != m_umResources.end())
            return &m_umResources.at(identifier);

        LOG_INFO("Loading resource %s", identifier.Raw().data());

        TResource *resource = CreateEmpty(identifier);
        m_Loader.Load(resource, identifier);

        return &m_umResources.at(identifier);
    }

    void Destroy(TResource **ppResource) {
        if (ppResource == nullptr)
            return;

        if (*ppResource == nullptr)
            return;

        m_umResources.erase(**ppResource);

        *ppResource = nullptr;
    }

    void Destroy(Identifier const &identifier) {
        m_umResources.erase(identifier);
    }

    bool Has(Identifier const &identifier) {
        return m_umResources.find(identifier) != m_umResources.end();
    }

    typename std::unordered_map<Identifier, TResource>::iterator begin() {
        return m_umResources.begin();
    }
    typename std::unordered_map<Identifier, TResource>::iterator end() {
        return m_umResources.end();
    }

protected:
    TResource *CreateEmpty(Identifier const &identifier) {
        m_umResources[identifier] = {};

        return &m_umResources.at(identifier);
    }

private:
    ILoader m_Loader;
    std::unordered_map<Identifier, TResource> m_umResources;
};
