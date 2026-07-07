#include <iostream>

#include "Data/Components.h"
#include "Logic/Scene.h"

int main() {
  Scene scene;
  std::cout << "start\n";
  Entity player = scene.createEntity();
  Entity enemy = scene.createEntity();

  TransformComponent* player_transform =
      scene.addComponent<TransformComponent>(player);
  TransformComponent* enemy_transform =
      scene.addComponent<TransformComponent>(enemy);

  player_transform->position.x = 0.0f;
  player_transform->position.y = 0.0f;
  player_transform->position.z = 0.0f;

  enemy_transform->position.x = 10.0f;
  enemy_transform->position.y = -5.0f;
  enemy_transform->position.z = 0.0f;

  scene.addComponent<HealthComponent>(player);
  scene.addComponent<HealthComponent>(enemy);

  std::cout << "player's start position:\nx: " << player_transform->position.x
            << " y: " << player_transform->position.y << "\n";
  std::cout << "enemy's start position:\nx: " << enemy_transform->position.x
            << " y: " << enemy_transform->position.y << "\n";

  float delta = 0.2f;

  for (int frame = 1; frame <= 5; ++frame) {
    scene.update(delta);
    std::cout << "Frame number " << frame << ":\n";
    std::cout << "Player position: " << player_transform->position.x << " "
              << player_transform->position.y << "\n";
    std::cout << "Enemy position: " << enemy_transform->position.x << " "
              << enemy_transform->position.y << "\n";
  }

  std::cout << "player's last position:\nx: " << player_transform->position.x
            << " y: " << player_transform->position.y << "\n";
  std::cout << "enemy's last position:\nx: " << enemy_transform->position.x
            << " y: " << enemy_transform->position.y << "\n";

  return 0;
}