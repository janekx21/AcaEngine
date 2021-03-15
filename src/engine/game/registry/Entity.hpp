#pragma once
#include <cstdint>

struct Entity {
	uint32_t id;
	uint32_t archetype; //position of entities archetype in archetype-vector
};
inline bool operator == (const Entity& a, const Entity& b) { return a.id == b.id; }
inline bool operator < (const Entity& a, const Entity& b) { return a.id < b.id; }