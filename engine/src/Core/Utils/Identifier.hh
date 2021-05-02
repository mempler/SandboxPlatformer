#pragma once

#include <EASTL/algorithm.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>

#include <algorithm>
#include <string>

class Identifier {
public:
    // E.G file://Texture.png
    // E.G pkg://Texture.png
    // E.G http://Texture.png
    constexpr Identifier(eastl::string_view const &svUri) {
        constexpr eastl::string_view protoEnd("://");

        auto proto_end = svUri.find_first_of(protoEnd);
        if (proto_end == eastl::string_view::npos) {
            m_sProtocol = svUri;
            return;
        }

#if _MSC_FULL_VER // m$ft moment
        m_sProtocol = eastl::string_view(&*svUri.begin(), proto_end);
        m_sPath = eastl::string_view(&*svUri.begin() + proto_end + protoEnd.size());
#else
        m_sProtocol = eastl::string_view(svUri.begin(), proto_end);
        m_sPath = eastl::string_view(svUri.begin() + proto_end + protoEnd.size());
#endif
    }

    constexpr Identifier(char *szUri) : Identifier(eastl::string_view(szUri)) {
    }
    constexpr Identifier(const char *szUri) : Identifier(eastl::string_view(szUri)) {
    }
    Identifier(eastl::string const &szUri) : Identifier(eastl::string_view(szUri)) {
    }

    constexpr eastl::string_view const &Protocol() const {
        return m_sProtocol;
    }

    constexpr eastl::string_view const Path() const {
        return m_sPath;
    }

    eastl::string Raw() const {
        return eastl::string(m_sProtocol) + "://" + m_sPath.data();
    }

    operator eastl::string() const {
        return Raw();
    }

    constexpr bool operator==(const Identifier &other) const {
        return (m_sPath == other.m_sPath && m_sProtocol == other.m_sProtocol);
    }

private:
    eastl::string_view m_sProtocol = "", m_sPath = "";
};

// std::unordered_map support
template <>
struct std::hash<Identifier> {
    std::size_t operator()(const Identifier &s) const noexcept {
        size_t h1 = eastl::hash<eastl::string_view>{}(s.Protocol().data());
        size_t h2 = eastl::hash<eastl::string_view>{}(s.Path().data());

        return h1 ^ (h2 << 1);
    }
};

// eastl::unordered_map support
template <>
struct eastl::hash<Identifier> {
    size_t operator()(const Identifier &s) const noexcept {
        size_t h1 = eastl::hash<eastl::string_view>{}(s.Protocol().data());
        size_t h2 = eastl::hash<eastl::string_view>{}(s.Path().data());

        return h1 ^ (h2 << 1);
    }
};
