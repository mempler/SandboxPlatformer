#pragma once

#include <string_view>

#include <algorithm>
#include <iterator>
#include <string>

class Identifier {
public:
    // E.G file://Texture.png
    // E.G pkg://Texture.png
    // E.G http://Texture.png
    constexpr Identifier(std::string_view const &svUri) {
        constexpr std::string_view protoEnd("://");

        auto proto_end = svUri.find_first_of(protoEnd);
        if (proto_end == std::string_view::npos) {
            m_sProtocol = svUri;
            return;
        }

#if _MSC_FULL_VER // cum shit cum shit cum shit cum shit cum shit cum shit cum shit cum shit cum shit cum shit cum shit 
        m_sProtocol = std::string_view(&*svUri.begin(), proto_end);
        m_sPath = std::string_view(&*svUri.begin() + proto_end + protoEnd.size());
#else
        m_sProtocol = std::string_view(svUri.begin(), proto_end);
        m_sPath = std::string_view(svUri.begin() + proto_end + protoEnd.size());
#endif
    }

    constexpr Identifier(char *szUri) : Identifier(std::string_view(szUri)) {
    }
    constexpr Identifier(const char *szUri) : Identifier(std::string_view(szUri)) {
    }
    Identifier(std::string const &szUri) : Identifier(std::string_view(szUri)) {
    }

    constexpr std::string_view const &Protocol() const {
        return m_sProtocol;
    }

    constexpr std::string_view const &Path() const {
        return m_sPath;
    }

    std::string Raw() const {
        return std::string(m_sProtocol) + "://" + m_sPath.data();
    }

    operator std::string() const {
        return Raw();
    }

    constexpr bool operator==(const Identifier &other) const {
        return (m_sPath == other.m_sPath && m_sProtocol == other.m_sProtocol);
    }

private:
    std::string_view m_sProtocol = "", m_sPath = "";
};

// std::unordered_map support
template <>
struct std::hash<Identifier> {
    std::size_t operator()(const Identifier &s) const noexcept {
        std::size_t h1 = std::hash<std::string_view>{}(s.Protocol());
        std::size_t h2 = std::hash<std::string_view>{}(s.Path());

        return h1 ^ (h2 << 1);
    }
};
