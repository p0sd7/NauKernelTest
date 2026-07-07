#include "Scene.h"

Scene::Scene() = default;
Scene::~Scene() = default;

Entity Scene::createEntity() {
  Entity entity{next_entity_id++};
  entities.push_back(entity);
  return entity;
}

void Scene::removeEntity(Entity ent) {
  for (auto& [type, pool] : component_vault) {
    pool->remove(ent);
  }
  std::erase(entities, ent);
}

void Scene::update(float delta) { movement_system.update(*this, delta); }