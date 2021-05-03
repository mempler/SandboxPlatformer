#pragma once

#include "Core/Utils/Identifier.hh"

#include <entt.hpp>

namespace Components {
    struct BaseComponent {
        Identifier m_Identifier;

        bool m_bIsDisabled;
        entt::entity m_iParent;
    };
} // namespace Components
