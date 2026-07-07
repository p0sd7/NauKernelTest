#ifndef ENTITY_H
#define ENTITY_H

struct Entity {
  uint32_t id;
  bool operator==(const Entity& other) const { return id == other.id; }
};

#endif