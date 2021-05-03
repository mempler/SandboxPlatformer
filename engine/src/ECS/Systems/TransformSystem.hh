#pragma once

#include "ECS/ISystem.hh"

namespace Systems {
    class TransformSystem final : public ISystem {
    public:
        explicit TransformSystem(entt::registry *pRegistry) : ISystem(pRegistry) {
        }

        void Draw(float fDelta) override;
        void Tick(float fDelta) override;
    };
} // namespace Systems
