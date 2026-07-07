#ifndef SCENE_H
#define SCENE_H

#include "../Entities/Data/IComponent.h"
#include "../Entities/Entity.h"

class Scene {
 public:
  void CreateEntity();
  void RemoveEntity();
  void AddComponent(Entity ent, ComponentType type);
  Component GetComponent(ComponentType type);
  bool CheckComponent(ComponentType type);
};

#endif