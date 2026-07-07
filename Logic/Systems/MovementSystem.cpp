#include "MovementSystem.h"

#include "../Scene.h"

void MovementSystem::update(Scene& scene, float delta) {
  const auto& transform_components =
      scene.getAllComponents<TransformComponent>();
  for (const auto& [entity, transform] : transform_components) {
    transform->position.x += move_direction_x * delta;
    transform->position.y += move_direction_y * delta;
  }
}