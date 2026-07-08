#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <nlohmann/json.hpp>
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

  std::unordered_map<std::type_index,
                     std::unique_ptr<IComponentPool>>
      component_vault;  // хэшмапа component_type-ComponentPool
  std::vector<Entity> entities;
  MovementSystem movement_system;

  template <typename T>
  ComponentPool<T>*
  getOrCreatePool();  // владение ComponentPool реализуется через unique_ptr,
                      // сырой указатель юзается для наблюдения
  template <typename T>
  const ComponentPool<T>* getPool()
      const;  // константный метод для чистого получения пула без изменения
              // сцены(ну т.е. если есть пул то возвращаем если нет то не
              // создаем и не возвращаем)

 public:
  Scene();
  ~Scene();

  using AllComponentTypes = std::tuple<TransformComponent, HealthComponent>;

  Entity createEntity();
  void removeEntity(Entity entity);

  void saveToFile(const std::string& filename);
  bool loadFromFile(const std::string& filename);

  void update(float delta);

  template <typename T>
  T* addComponent(Entity ent);

  template <typename T>
  const T* getComponent(Entity ent) const;

  template <typename T>
  T* getComponent(Entity ent);

  const std::vector<Entity>& getEntities() const { return entities; }

  template <typename T>
  bool checkComponent(Entity ent) const;

  template <typename T>
  std::unordered_map<uint32_t, std::unique_ptr<T>>& getAllComponents();
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
const Scene::ComponentPool<T>* Scene::getPool() const {
  auto type_index = std::type_index(typeid(T));
  auto it = component_vault.find(type_index);
  if (it == component_vault.end()) {
    return nullptr;
  }
  return static_cast<const ComponentPool<T>*>(it->second.get());
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
const T* Scene::getComponent(Entity ent) const {
  const auto* pool = getPool<T>();
  if (!pool) return nullptr;

  auto it = pool->data.find(ent.id);
  if (it == pool->data.end()) return nullptr;
  return it->second.get();
};

template <typename T>
T* Scene::getComponent(Entity ent) {  // перегрузка на случай когда данные надо
                                      // изменить(добавить пул)
  auto* pool = getOrCreatePool<T>();
  auto it = pool->data.find(ent.id);
  if (it == pool->data.end()) return nullptr;
  return it->second.get();
};

template <typename T>
bool Scene::checkComponent(Entity ent) const {
  auto* pool = getPool<T>();
  if (!pool) return false;
  return pool->data.contains(ent.id);
};

template <typename T>
std::unordered_map<uint32_t, std::unique_ptr<T>>& Scene::getAllComponents() {
  auto* pool = getOrCreatePool<T>();
  return pool->data;
};

template <typename Tuple, size_t index = 0>
void serializeComponents(const Scene& scene, Entity entity,
                         nlohmann::json& entity_json) {
  if constexpr (index >= std::tuple_size_v<Tuple>) {
    return;
  } else {
    using CurrentComponent = std::tuple_element_t<index, Tuple>;
    std::string type_name = typeid(CurrentComponent).name();

    if (scene.checkComponent<CurrentComponent>(entity)) {
      const auto* component = scene.getComponent<CurrentComponent>(entity);
      entity_json["components"][type_name] = *component;
    }

    if constexpr (index + 1 < std::tuple_size_v<Tuple>) {
      serializeComponents<Tuple, index + 1>(scene, entity, entity_json);
    }
  }
};

template <typename Tuple, size_t index = 0>
void deserializeComponents(Scene& scene, Entity entity,
                           const nlohmann::json& entity_json) {
  if constexpr (index >= std::tuple_size_v<Tuple>) {
    return;
  } else {
    using CurrentComponent = std::tuple_element_t<index, Tuple>;
    std::string type_name = typeid(CurrentComponent).name();

    if (entity_json.contains("components") &&
        entity_json["components"].contains(type_name)) {
      auto* component = scene.addComponent<CurrentComponent>(entity);
      entity_json["components"][type_name].get_to(*component);
    }
    if constexpr (index + 1 < std::tuple_size_v<Tuple>) {
      deserializeComponents<Tuple, index + 1>(scene, entity, entity_json);
    }
  }
};

#endif