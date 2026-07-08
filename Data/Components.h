#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <nlohmann/json.hpp>

struct IComponent {
  virtual ~IComponent() = default;
};

// Transform component
struct Vector3 {
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z)

struct TransformComponent : IComponent {
  Vector3 position;
  Vector3 rotation;
  Vector3 scale{1.0f, 1.0f, 1.0f};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformComponent, position, rotation,
                                   scale)

// Health component
struct HealthComponent : IComponent {
  float max_health = 100.0f;
  float current_health = 100.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HealthComponent, max_health, current_health)

#endif