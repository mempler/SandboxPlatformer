#pragma once

#include <IceSDK/ECS.hh>

namespace IceSDK::_internal
{
    void TransformSystem( float fDelta, Scene &scene, entt::registry &registry );
}