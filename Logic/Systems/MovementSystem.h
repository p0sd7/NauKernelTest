#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../../Data/Components.h"

class Scene;  // Forward declaration разрешает циклическую зависимость, ускоряет
              // компиляцию, архитектуру не ломает

class MovementSystem {
 private:
  float move_direction_x = 1.0f;
  float move_direction_y = 1.0f;

 public:
  void update(
      Scene& scene,
      float delta);  // здесь по ссылке берем как раз потому что forward
                     // declaration(тк не знаем размер типа), не крашнется тк
                     // сцена существует в рантайме постоянно
};

#endif