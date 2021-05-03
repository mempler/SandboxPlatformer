#include "pch.hh"

#include "Scene.hh"

#include "Systems/TransformSystem.hh"

#include "Components/BaseComponent.hh"
#include "Components/TransformComponent.hh"

Scene::Scene() {
    RegisterSystem<Systems::TransformSystem>();
}

Scene::~Scene() {
    // Cleanup Systems
    for (auto &&system : m_vpSystems) {
        delete system;
    }
}

Entity Scene::CreateEntity(Identifier const &identifier) {
    auto entity = Entity(GetRegistry());

    entity.AttachComponent<Components::BaseComponent>(identifier, false, entt::null);

    return entity;
}

void Scene::Tick(float fDelta) {
    for (auto system : m_vpSystems) {
        system->Tick(fDelta);
    }
}

void Scene::Draw(float fDelta) {
    for (auto system : m_vpSystems) {
        system->Draw(fDelta);
    }
}