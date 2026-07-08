#include "Scene.h"

#include <fstream>

using json = nlohmann::json;

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::saveToFile(const std::string& filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cout << "Save file cannot be opened";
    return;
  }
  json root = json::array();
  for (const auto& entity : entities) {
    json entity_json;
    entity_json["id"] = entity.id;
    serializeComponents<AllComponentTypes>(*this, entity, entity_json);
    root.push_back(entity_json);
  }
  file << root.dump(4);
  if (file.fail()) {
    std::cout << "Save error: " << filename << "\n";
  }
}

bool Scene::loadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "Save file not found: " << filename << "\n";
    return false;
  }
  json root;
  try {
    file >> root;
    component_vault.clear();
    next_entity_id = 1;

    for (const auto& entity_json : root) {
      uint32_t saved_id = entity_json["id"].get<uint32_t>();
      Entity entity{saved_id};
      entities.push_back(entity);
      if (saved_id >= next_entity_id) {
        next_entity_id = saved_id + 1;
      }
      deserializeComponents<AllComponentTypes>(*this, entity, entity_json);
    }
    std::cout << "Scene is loaded from file susccessful: " << filename << "\n";
    return true;
  } catch (const std::exception& e) {
    std::cout << "Incorrect save file\n";
    std::cout << "Details: " << e.what() << "\n";
    std::cout << "New game started\n";
  }
  return false;
}

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