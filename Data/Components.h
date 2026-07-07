#ifndef COMPONENTS_H
#define COMPONENTS_H

struct IComponent {
  virtual ~IComponent() = default;
};

// Transform component
struct Vector3 {
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
};

struct TransformComponent : IComponent {
  Vector3 position;
  Vector3 rotation;
  Vector3 scale{1.0f, 1.0f, 1.0f};
};

// Health component
struct HealthComponent : IComponent {
  float max_health = 100.0f;
  float current_health = 100.0f;
};

#endif