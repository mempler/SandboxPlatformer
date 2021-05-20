#include "Systems/TransformSystem.hh"

#include <IceSDK/ECS.hh>

void IceSDK::Scene::RegisterDefaultSystems()
{
    RegisterTickSystem( _internal::TransformSystem );
}
