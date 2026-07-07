#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <typeindex>

#include "../Data/Components.h"
#include "../Data/Entity.h"
#include "Systems/MovementSystem.h"

class Scene {
 private:
  uint32_t next_entity_id = 1;

  struct IComponentPool {
    virtual ~IComponentPool() = default;
    virtual void remove(Entity ent) = 0;
  };

  template <typename T>
  struct ComponentPool : IComponentPool {
    std::unordered_map<uint32_t, std::unique_ptr<T>>
        data;  // хэшмапа entity-component
    void remove(Entity ent) override { data.erase(ent.id); }
  };

  std::unordered_map<
      std::type_index,
      std::unique_ptr<IComponentPool>>  // хэшмапа component_type-ComponentPool
      component_vault;
  std::vector<Entity> entities;
  MovementSystem movement_system;

  template <typename T>
  ComponentPool<T>*
  getOrCreatePool();  // владение ComponentPool реализуется через unique_ptr,
                      // сырой указатель юзается для наблюдения

 public:
  Scene();
  ~Scene();

  Entity createEntity();
  void removeEntity(Entity entity);
  void update(float delta);

  template <typename T>
  T* addComponent(Entity ent);

  template <typename T>
  T* getComponent(Entity ent);

  template <typename T>
  bool checkComponent(Entity ent);

  template <typename T>
  const std::unordered_map<uint32_t, std::unique_ptr<T>>& getAllComponents();
};

template <typename T>
Scene::ComponentPool<T>* Scene::getOrCreatePool() {
  auto type_index = std::type_index(typeid(T));
  auto it = component_vault.find(type_index);
  if (it == component_vault.end()) {
    auto new_pool = std::make_unique<ComponentPool<T>>();
    ComponentPool<T>* raw_ptr = new_pool.get();
    component_vault[type_index] = std::move(new_pool);
    return raw_ptr;
  }
  return static_cast<ComponentPool<T>*>(it->second.get());
};

template <typename T>
T* Scene::addComponent(Entity ent) {
  auto* pool = getOrCreatePool<T>();
  auto component = std::make_unique<T>();
  T* raw_ptr = component.get();
  pool->data[ent.id] = std::move(component);
  return raw_ptr;
};

template <typename T>
T* Scene::getComponent(Entity ent) {
  auto* pool = getOrCreatePool<T>();
  auto it = pool->data.find(ent.id);
  if (it == pool->data.end()) return nullptr;
  return it->second.get();
};

template <typename T>
bool Scene::checkComponent(Entity ent) {
  auto* pool = getOrCreatePool<T>();
  return pool->data.contains(ent.id);
};

template <typename T>
const std::unordered_map<uint32_t, std::unique_ptr<T>>&
Scene::getAllComponents() {
  auto* pool = getOrCreatePool<T>();
  return pool->data;
};

#endif